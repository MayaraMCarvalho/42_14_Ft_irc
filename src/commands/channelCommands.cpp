/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macarval <macarval@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:02:58 by macarval          #+#    #+#             */
/*   Updated: 2024/07/22 13:23:47 by macarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void Commands::commandJoin( void )
{
	if (initValidation(2))
	{
		std::string	channel = _args[1];
		std::string	key = "";

		if (_args.size() > 2)
			key = _args[2];

		if (validChannelName(channel) && validArg(channel)
			&& verifyJoin(channel, key))
		{
			_channels.join(_fd, channel, key);
			printJoin(channel);
		}
	}
}

void Commands::printJoin(std::string channelName)
{
	Client client = _clients.getClient(_fd)->second;
	std::string message = ":" + client.getFullId() + " " + _args[0] + " :" + channelName;

	sendMessage(_channels.get(channelName), message);

	printInfo(getTopic(channelName));
	printInfo(getNamReply(channelName));
	printInfo(getEndOfNames(channelName));
}

void Commands::commandPart( void )
{
	if (initValidation(2))
	{
		std::string channel = _args[1];

		if (validChannelName(channel) && validArg(channel)
			&& verifyChannel(channel))
		{
			_channels.part(_fd, channel);
			printInfo(GREEN + "User successfully part the channel " +
				channel + "!" + RESET);// Verificar
		}
	}
}

void Commands::commandKick( void )
{
	if (initValidation(3))
	{
		std::string	channel = _args[1];
		std::string	user = _args[2];

		if (_args.size() > 3)
			std::string	comment = getMessage(3);

		if (validChannelName(channel) && validArg(channel) &&
			validArg(user) && verifyChannel(channel) && verifyChanOp(channel))
		{
			_channels.part(_clients.getFDByUser(user), channel);
			printInfo(PURPLE + "The user " + BYELLOW + user +
				PURPLE + " have been removed from the channel " +
				BYELLOW + channel + PURPLE + "by the operator " +
				BYELLOW + _clients.getUser(_fd) + "!" + RESET);// Verificar
		}
	}
}
