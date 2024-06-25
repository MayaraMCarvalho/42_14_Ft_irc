/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelList.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmachado <gmachado@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 01:12:12 by gmachado          #+#    #+#             */
/*   Updated: 2024/06/25 15:07:04 by gmachado         ###   ########.fr       */
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

		std::map<std::string, Channel>::iterator add(Channel channel);
		std::map<std::string, Channel>::size_type remove(std::string name);

	public:
		ChannelList(void);
		ChannelList(ClientList *clients);
		ChannelList(ChannelList &src);

		~ChannelList(void);

		ChannelList &operator=(ChannelList &src);

		// Getters
		std::map<std::string, Channel>::iterator get(std::string name);
		std::map<std::string, Channel>::iterator end(void);

		void join(int userFD, const std::string &chanName, const std::string &key);
		void part(int userFD, std::string chanName);
		void partDisconnectedClient(int userFD);
		bool userCanJoin(int userFD, Channel &chan, const std::string &key);
};

#endif