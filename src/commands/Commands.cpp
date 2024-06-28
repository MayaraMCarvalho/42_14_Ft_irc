/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macarval <macarval@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 13:47:14 by macarval          #+#    #+#             */
/*   Updated: 2024/06/28 17:02:53 by macarval         ###   ########.fr       */
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

// Setters ====================================================================

// Methods ====================================================================
bool Commands::isCommand(int clientFd, const std::string &message)
{
	std::map<std::string, void (Commands::*)()> cmdFuncs;
	_fd = clientFd;

	cmdFuncs[PASS] = &Commands::commandPass; // Ok
	cmdFuncs[NICK] = &Commands::commandNick; // Ok
	cmdFuncs[USER] = &Commands::commandUser; // Ok
	cmdFuncs[JOIN] = &Commands::commandJoin; //
	cmdFuncs[PART] = &Commands::commandPart; //
	cmdFuncs[PRIVMSG] = &Commands::commandPrivMsg; //
	cmdFuncs[KICK] = &Commands::commandKick;
	cmdFuncs[INVITE] = &Commands::commandInvite;
	cmdFuncs[TOPIC] = &Commands::commandTopic;
	cmdFuncs[MODE] = &Commands::commandMode;
	cmdFuncs[QUIT] = &Commands::commandQuit; //

	parsingArgs(message);

	std::map<std::string, void (Commands::*)()>::iterator it = cmdFuncs.find(_args[0]);
	if (it != cmdFuncs.end())
	{
		(this->*(it->second))();
		return true;
	}

	return false;
}

void Commands::commandKick( void )
{
	if (initialVerify(3, "KICK <channel> <user> <comment(Optional)>\n"))
	{
		std::string channel = _args[1];
		std::string user = _args[2];
		if (_args.size() > 3)
			std::string comment = getMessage(3);

		if (!validChannel(channel) || !validArg(channel) ||
			!validArg(user) || !verifyKick(channel, user))
			return ;

		_channels.part(_clients.getFDByUser(user), channel);
		printError(PURPLE + "The user " + BYELLOW + user +
			PURPLE + " have been removed from the channel " +
			BYELLOW + channel + PURPLE + "by the operator " +
			BYELLOW + _clients.getUser(_fd) + "!\n" + RESET);

	}
}

bool Commands::verifyKick(std::string &channel, std::string &user)
{
	if (!_clients.getClient(_fd)->second.getMode(Client::OPERATOR))
	{
		printError(RED + "Error " + codeToString(ERR_CHANOPRIVSNEEDED) +
			": #" + channel + ":You're not channel operator\n" + RESET);
	}
	else if (_channels.get(channel) == _channels.end())
	{
		printError(RED + "Error "+ codeToString(ERR_NOSUCHCHANNEL) +
			":Channel not found\n" + RESET);
	}
	else if (user.empty())
	{
		printError(RED + "Error "+ codeToString(ERR_NOSUCHCHANNEL) +
			":Channel not found\n" + RESET);
	}
	else
		return true;
	return false;
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
