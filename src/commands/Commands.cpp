/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmachado <gmachado@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 13:47:14 by macarval          #+#    #+#             */
/*   Updated: 2024/07/02 19:00:25 by gmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "IrcServer.hpp"

// Constructor & Destructor ===================================================
Commands::Commands( IRCServer& server ) : _server(server),
	_clients(_server.getClients()), _channels(_server.getChannels()),
	_serverPass(_server.getPassword()) {}

Commands::~Commands(void) {}

// Getters ====================================================================
std::vector<std::string> Commands::getArgs(void) const {
	return _args;
}

ClientList& Commands::getClients(void) const {
	return _clients;
}

ChannelList& Commands::getChannels(void) const {
	return _channels;
}

int Commands::getFd(void) const {
	return _fd;
}
// Setters ====================================================================
void Commands::setArgs(const std::vector<std::string> &args) {
	_args = args;
}

void Commands::setFd(int fd) {
	_fd = fd;
}

// Methods ====================================================================
bool Commands::isCommand(int clientFd, const std::string &message)
{
	std::map<std::string, void (Commands::*)()> cmdFuncs;

	_fd = clientFd;
	cmdFuncs[PASS] = &Commands::commandPass; // Ok
	cmdFuncs[NICK] = &Commands::commandNick; // Ok
	cmdFuncs[USER] = &Commands::commandUser; // Ok
	cmdFuncs[JOIN] = &Commands::commandJoin; //			F1
	cmdFuncs[PART] = &Commands::commandPart; // Ok
	cmdFuncs[PRIVMSG] = &Commands::commandPrivMsg; //	F1
	cmdFuncs[KICK] = &Commands::commandKick; // Ok
	cmdFuncs[INVITE] = &Commands::commandInvite;
	cmdFuncs[TOPIC] = &Commands::commandTopic;
	cmdFuncs[MODE] = &Commands::commandMode;
	cmdFuncs[QUIT] = &Commands::commandQuit; // Ok

	parsingArgs(message);
	std::map<std::string, void (Commands::*)()>::iterator it =
		cmdFuncs.find(_args[0]);
	if (it != cmdFuncs.end())
	{
		(this->*(it->second))();
		return true;
	}
	return false;
}

void Commands::parsingArgs(const std::string &message)
{
	std::string			token;
	std::istringstream	tokenStream(message);

	while (std::getline(tokenStream, token, ' '))
		_args.push_back(token);
}

void Commands::commandInvite( void )
{
 std::cout << "Command Invite" << std::endl;
}

void Commands::commandTopic( void )
{
	std::cout << "Command Topic" << std::endl;
}

void Commands::commandMode( void )
{
	std::cout << "Command Mode" << std::endl;
}
