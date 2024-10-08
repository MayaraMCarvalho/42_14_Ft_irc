/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macarval <macarval@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 13:47:14 by macarval          #+#    #+#             */
/*   Updated: 2024/08/22 12:38:03 by macarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Colors.hpp"
#include "IrcServer.hpp"

// Constructor & Destructor ===================================================
Commands::Commands( IRCServer& server ) : _server(server),
	_clients(_server.getClients()), _channels(_server.getChannels()),
	_serverPass(_server.getPassword()),
	_host(_server.getMsgHandler().getHost()) {}

Commands::~Commands(void) {}

// Methods ====================================================================

void Commands::extractCommands(int clientFd) {
	MsgHandler &msgHandler = _server.getMsgHandler();

	if (msgHandler.recvLength(clientFd) < 1)
		return;
	try {
		std::string &str = msgHandler.recvPop(clientFd);

		size_t startIdx = 0;
		size_t endIdx = str.find("\r\n");

		if (endIdx == std::string::npos &&
				(str.find('\n') != std::string::npos ||
					str.find('\r') != std::string::npos)) {
			_server.getLogger().warn(RED +
				"Message contains invalid line end characters: " + BYELLOW +
				str + RESET);
			str.clear();
			return;
		}

		while (endIdx != std::string::npos) {
			if (!isCommand(clientFd,
					str.substr(startIdx, endIdx - startIdx))) {
				sendMessage(clientFd,
					_server.getMsgHandler().errUnknownCommand(
						_clients.getNick(clientFd),
						str.substr(startIdx, endIdx - startIdx),
						"unknown command"));
			}

			if (_server.getIsFdDisconnected())
				return;

			startIdx = endIdx + 2;
			endIdx = str.find("\r\n", startIdx);
		}

		if (startIdx == 0 || _server.getIsFdDisconnected())
			return;

		if (startIdx < str.length())
			str = str.substr(startIdx);
		else
			str.clear();

	} catch(std::out_of_range &e) {
		_server.getLogger().warn(RED +
			"Out of range exception caught while processing client messages" +
			RESET);
	}
}

bool Commands::isCommand(int clientFd, const std::string &message)
{
	std::map<std::string, void (Commands::*)()> cmdFuncs;

	_fd = clientFd;
	cmdFuncs[WHO] = &Commands::commandWho;
	cmdFuncs[PASS] = &Commands::commandPass;
	cmdFuncs[NICK] = &Commands::commandNick;
	cmdFuncs[USER] = &Commands::commandUser;
	cmdFuncs[JOIN] = &Commands::commandJoin;
	cmdFuncs[PART] = &Commands::commandPart;
	cmdFuncs[KICK] = &Commands::commandKick;
	cmdFuncs[MODE] = &Commands::commandMode;
	cmdFuncs[QUIT] = &Commands::commandQuit;
	cmdFuncs[TOPIC] = &Commands::commandTopic;
	cmdFuncs[INVITE] = &Commands::commandInvite;
	cmdFuncs[PRIVMSG] = &Commands::commandPrivMsg;

	_server.getLogger().debug(CYAN + "Received message from client " +
		itoa(clientFd) + ": " + BYELLOW + message + RESET);

	_args.clear();
	parsingArgs(message, ' ', _args);

	if (_args.size() > 0)
	{
		strToUpper(_args[0]);
		std::map<std::string, void (Commands::*)()>::iterator it =
			cmdFuncs.find(_args[0]);
		if (it != cmdFuncs.end())
		{
			(this->*(it->second))();
			return true;
		}
	}
	return false;
}

void Commands::parsingArgs(const std::string &message, char c,
						   std::vector<std::string>	&vector)
{
	std::string			token;
	std::istringstream	tokenStream(message);

	while (std::getline(tokenStream, token, c))
	{
		if (!token.empty())
			vector.push_back(token);
	}
}

void Commands::commandTopic( void )
{
	if (validSetup() && initValidation(2))
	{
		std::string	channelName = _args[1];
		std::string	topic = "";

		if (validArg(channelName)&& validChannelName(channelName)
			&& verifyChannel(channelName))
		{
			if (_args.size() > 2)
			{
				topic = getMessage(2);
				if (verifyChanOp(channelName) && validMessage(topic))
				{
					_channels.get(channelName)->second.setTopic(topic);
					std::string from = _clients.getClient(_fd)->second.getFullId();
					std::string message = " TOPIC " + channelName + " :" + topic;
					sendMessage(_channels.get(channelName), message, from);
				}
			}
			else
				printInfo(getTopic(channelName));
		}
	}
}
