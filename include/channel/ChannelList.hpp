/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelList.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macarval <macarval@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 01:12:12 by gmachado          #+#    #+#             */
/*   Updated: 2024/06/27 15:22:54 by macarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_LIST_HPP
# define CHANNEL_LIST_HPP

# include <map>
# include <iostream>
# include "Channel.hpp"
# include "ClientList.hpp"
# include "Colors.hpp"

class ChannelList
{
	private:
		std::map<std::string, Channel> _channels;
		ClientList *_clients;
		static const int _DEFAULT_FLAGS = Channel::NO_UMODE;
		std::map<std::string, std::set<std::string> > _invites;

		std::map<std::string, Channel>::iterator add(Channel channel);
		std::map<std::string, Channel>::size_type remove(std::string name);

		void addInvite(const std::string &nick, const std::string &chan);
		void removeInvite(const std::string &nick, const std::string &chan);

	public:
		ChannelList(void);
		ChannelList(ClientList *clients);
		ChannelList(ChannelList &src);

		~ChannelList(void);

		ChannelList &operator=(ChannelList &src);

		// Getters
		std::map<std::string, Channel>::iterator get(std::string name);
		std::map<std::string, Channel>::iterator begin(void);
		std::map<std::string, Channel>::iterator end(void);

		void join(int userFD, const std::string &chanName, const std::string &key);
		void part(int userFD, std::string chanName);
		void partDisconnectedClient(int userFD);
		bool userCanJoin(int userFD, Channel &chan, const std::string &key);
		bool userHasInvite(const std::string &nick, const std::string &chan);
		t_numCode inviteUser(const std::string &inviter,
			const std::string &invitee, const std::string &chan);
};

#endif
