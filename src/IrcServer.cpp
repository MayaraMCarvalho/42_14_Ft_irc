/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macarval <macarval@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:58:55 by macarval          #+#    #+#             */
/*   Updated: 2024/08/22 21:23:57 by macarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "Commands.hpp"
#include "Colors.hpp"
#include <cstdio>
#include <sys/types.h>
#include <netdb.h>

IRCServer* IRCServer::_instance = NULL;

// Constructor & Destructor ===================================================
IRCServer::IRCServer(const std::string &port, const std::string &password,
		Logger &logger)
	: _port(port), _password(password), _serverFd(-1),
	_logger(logger), _msgHandler(_logger), _clients(_msgHandler, _pollFds),
	_channels(_clients, _msgHandler), _shouldExit(false)
{
	_instance = this;
}

IRCServer::~IRCServer(void) { }

// Getters ====================================================================
ClientList& IRCServer::getClients( void ) { return _clients; }

ChannelList& IRCServer::getChannels( void ) { return _channels; }

const std::string& IRCServer::getPassword( void ) { return _password; }

Logger &IRCServer::getLogger(void) { return _logger; }

// Setters ====================================================================

// Methods ====================================================================

void IRCServer::setupServer(void)
{
	struct sockaddr_in address;
	int	opt;

	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(std::atoi(_port.c_str()));

	_serverFd = socket(AF_INET, SOCK_STREAM, 0);

	if (_serverFd < 0)
		throw std::runtime_error("Failed to create socket");

	opt = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("Failed to set socket options");

	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Unable to set non-blocking mode on client file descriptor (fcntl)");
	if (bind(_serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("Failed to bind socket");

	if (listen(_serverFd, 10) < 0)
		throw std::runtime_error("Failed to listen on socket");

	struct pollfd pfd = {_serverFd, POLLIN | POLLOUT, STDIN_FILENO};
	_pollFds.push_back(pfd);
}

void IRCServer::signalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM || signal == SIGTSTP)
	{
		ChannelList	channels = _instance->_channels;
		ClientList	clients = _instance->_clients;
		_instance->_shouldExit = true;
	}
}

void IRCServer::setupSignalHandlers(void)
{
	struct sigaction action;
	std::memset(&action, 0, sizeof(action));
	action.sa_handler = signalHandler;

	if (sigaction(SIGINT, &action, NULL) == -1 ||
		sigaction(SIGTERM, &action, NULL) == -1 ||
		sigaction(SIGTSTP, &action, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

void IRCServer::run(void)
{
	int	pollCount;

	setupServer();
	setupSignalHandlers();
	_logger.info(GREEN + "Server running on port " +
		BYELLOW + itoa(_port) + RESET);

	while (!_shouldExit)
	{
		sigset_t		pendingSignals;

		if (sigpending(&pendingSignals) == 0)
		{
			if (sigismember(&pendingSignals, SIGINT)
			|| sigismember(&pendingSignals, SIGTERM)
			|| sigismember(&pendingSignals, SIGTSTP))
				break;
		}

		pollCount = poll(_pollFds.data(), _pollFds.size(), 0);
		if (pollCount < 0)
				throw std::runtime_error("Poll error");

		if (_pollFds[0].revents & POLLIN)
			acceptNewClient();

		size_t fdIdx = 1;
		size_t pollFdsSize = _pollFds.size();

		while (fdIdx < pollFdsSize) {
			int fd = _pollFds[fdIdx].fd;
			_isFdDisconnected = false;

			if (_pollFds[fdIdx].revents & POLLIN &&
					!handleClientMessage(_pollFds[fdIdx].fd)) {
				disconnectClient(fd, fdIdx);
			}
			else if (_pollFds[fdIdx].revents & POLLOUT) {
				ssize_t strLen = _msgHandler.sendLength(fd);

				if (strLen > 0) {
					try {
						ssize_t	nbytes = send(fd,
							_msgHandler.sendPop(fd).c_str(),strLen, 0);

						if (nbytes <= 0) {
							_logger.error(RED + "Write error on client " +
								BYELLOW + itoa(fd) + RESET);
							disconnectClient(fd, fdIdx);
						}
						else
							_msgHandler.removeSendChars(fd, nbytes);
					} catch (std::out_of_range &e) {
						_logger.warn(RED +
							"Out of range exception caught while processing " +
							"message queue" + RESET);
					}
				}
			}
			if (_isFdDisconnected)
				pollFdsSize = _pollFds.size();
			else
				++fdIdx;
		}
	}
	_logger.debug(BGREEN + "Exiting gracefully!" + RESET);
}

void IRCServer::acceptNewClient(void)
{
	int	clientFd;
	struct sockaddr_in clientAddress;
	socklen_t clientLen = sizeof(clientAddress);
	clientFd = accept(_serverFd, (struct sockaddr *)&clientAddress, &clientLen);

	if (clientFd < 0) {
		_logger.error("Failed to accept new client");
		return;
	}

	if (fcntl(clientFd, F_SETFL, O_NONBLOCK)) {
		throw std::runtime_error("Unable to set non-blocking mode on client file descriptor (fcntl)");
	}

	_clients.add(clientFd);
	struct pollfd pfd = {clientFd, POLLIN | POLLOUT, 0};
	_pollFds.push_back(pfd);

	_logger.info(BLUE + "New client connected: " + BYELLOW +
		itoa(clientFd) + RESET);
}

bool IRCServer::handleClientMessage(int clientFd)
{
	char	buffer[MAX_MSG_LENGTH + 1];
	ssize_t	nbytes;

	nbytes = read(clientFd, buffer, sizeof(buffer) - 1); // read client data

	if (nbytes < 0) {
		_logger.error(RED + "Read error on client: " + BYELLOW +
			itoa(clientFd) + RESET);
		return false;
	}
	else if (nbytes == 0) {
		std::string quit = "QUIT :disconnected";
		for (std::map<std::string, Channel>::iterator it = _channels.begin();
			it != _channels.end(); ++it)
		{
			if (it->second.userIsInChannel(clientFd))
			{
				it->second.sendToAll(_clients.getClient(clientFd)->second.getFullId(),
									quit);
				std::map<std::string, Channel>::iterator next = it;
				++next;
				_channels.part(clientFd, it->second.getName());
				it = next;
			}
		}
		// disconnectClient(clientFd);

		_logger.info("Client disconnected: " + BYELLOW +
			itoa(clientFd) + RESET);
		return false;
	}

	buffer[nbytes] = '\0';

	if (!_msgHandler.recvPush(clientFd, buffer))
	{
		_logger.warn("Receive message queue is full" + RESET);
		return false;
	}

	Commands	commands(*this);

	commands.extractCommands(clientFd);
	return true;
}

std::string IRCServer::getHostName(const int socketFd) {


	struct sockaddr_in sockAddress;
	socklen_t sockLen = 0;
	struct addrinfo *addrInfo = NULL;

	memset(&sockAddress, 0, sizeof(sockAddress));
	getsockname(socketFd, (struct sockaddr *)&sockAddress, &sockLen);

	std::string strAddress(inet_ntoa(sockAddress.sin_addr));
	std::cerr << "Address: " << strAddress << std::endl;
	std::cerr << "Port: " << sockAddress.sin_port << std::endl;

	if (getaddrinfo(strAddress.c_str(),
			itoa(sockAddress.sin_port).c_str(),
			NULL, &addrInfo))
		return strAddress;

	std::cerr << "addrInfo address: " << addrInfo << std::endl;
	std::string hostName(addrInfo->ai_canonname);
	std::cerr << "Name: " << strAddress << std::endl;
	freeaddrinfo(addrInfo);
	return hostName;
}

void IRCServer::disconnectClient(int fd) {
	size_t fdIdx;

	for (fdIdx = 1; fdIdx < _pollFds.size(); ++fdIdx) {

		if (fd == _pollFds[fdIdx].fd) {
			break;
		}
	}
	if (fdIdx >= _pollFds.size())
	{
		_logger.error(RED + "Could not find fd in _pollfds: " + BYELLOW +
			itoa(fd) + RESET);
	}
	else
		disconnectClient(fd, fdIdx);
}

void IRCServer::disconnectClient(int fd, size_t fdIdx) {
	_clients.removeClientFD(fd);
	_channels.partDisconnectedClient(fd);
	_msgHandler.resetQueues(fd);

	if (fdIdx < _pollFds.size())
		_pollFds.erase(_pollFds.begin() + fdIdx);
	else
		_logger.error(RED + "Invalid fdIdx: " + BYELLOW + itoa(fdIdx) + RESET);

	if (close(fd) == -1)
		_logger.error(RED + "Failed to close fd: " + BYELLOW + itoa(fd) + RESET);
	else
		_isFdDisconnected = true;

}

MsgHandler &IRCServer::getMsgHandler(void) { return _msgHandler; }

bool IRCServer::getIsFdDisconnected(void) { return _isFdDisconnected; }
