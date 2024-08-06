/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infos.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macarval <macarval@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 10:59:16 by macarval          #+#    #+#             */
/*   Updated: 2024/08/05 11:20:02 by macarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

std::string Commands::getWelcome(Client &client)
{
	return (GREEN + toString(RPL_WELCOME) + " " + client.getNick() +
			" :Welcome to IRC server " + BYELLOW + client.getFullId() + RESET);
}

std::string Commands::getYourHost(Client &client)
{
	return (GREEN + toString(RPL_YOURHOST) + " " + client.getNick() +
			BYELLOW + " :Your host is " + _host + RESET);
}

std::string Commands::getMyInfo(Client &client)
{
	int	modeFlags = client.getModeFlags();

	return (GREEN + toString(RPL_MYINFO) + " " + client.getNick() +
			BYELLOW + " " + _host + " " + getUserFlags(modeFlags) + RESET);
}

std::string Commands::getUserModeIs(Client &client)
{
	int	modeFlags = client.getModeFlags();

	return (CYAN + toString(RPL_UMODEIS) + " " + client.getNick()
			+ GREEN + " " + getUserFlags(modeFlags) + RESET);
}

std::string Commands::getChannelModeIs(Channel &channel)
{
	int	modeFlags = channel.getChannelModeFlags();

	return (CYAN + toString(RPL_CHANNELMODEIS) + " " + channel.getName()
			+ GREEN + " " + getChannelFlags(modeFlags, channel) + RESET);
}

std::string Commands::getTopic(std::string &channelName)
{
	std::string topic = _channels.get(channelName)->second.getTopic();

	if (topic.empty())
		return (CYAN + toString(RPL_NOTOPIC) + " " + _clients.getNick(_fd)
				+ " " + channelName + " :No topic is set" + RESET);
	else
		return (CYAN + toString(RPL_TOPIC) + " " + _clients.getNick(_fd)
				+ " " + channelName + " :" + topic + RESET);

}

std::string Commands::getInviting(std::string &nickname,
								  std::string &channelName)
{
	return (CYAN + toString(RPL_INVITING) + " " + _clients.getNick(_fd)
			+ " " + nickname + " " + channelName + RESET);
}

std::string Commands::getNamReply(std::string &channelName)
{
	Channel		channel = _channels.get(channelName)->second;
	std::string	listUsers = "";

	for (std::map<int, int>::iterator it = channel.usersBegin();
		it != channel.usersEnd(); ++it)
	{
		if (it != channel.usersBegin() && it != channel.usersEnd())
			listUsers.append(" ");
		listUsers.append(_clients.getNick(it->first));
	}
	return (CYAN + toString(RPL_NAMREPLY) + " " + _clients.getNick(_fd)
			+ " = " + channelName + " :" + listUsers + "" + RESET);
}

std::string Commands::getEndOfNames(std::string &channelName)
{
	return (CYAN + toString(RPL_ENDOFNAMES) + " " + _clients.getNick(_fd)
			+ " " + channelName + " :End of /NAMES list." + RESET);
}