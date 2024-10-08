/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientList.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmachado <gmachado@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 01:30:27 by gmachado          #+#    #+#             */
/*   Updated: 2024/08/22 02:57:34 by gmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_LIST_HPP
# define CLIENT_LIST_HPP

# include <map>
# include "Client.hpp"
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
#include <vector>
# include "Codes.hpp"

class ClientList {
	public:
		ClientList(MsgHandler &msgHandler, std::vector<struct pollfd> &pollFds);
		ClientList(ClientList &src);

		~ClientList(void);

		ClientList &operator=(ClientList &src);

		// Getters
		std::map<int, Client>::iterator getClient(int fd);
		std::map<int, Client>::iterator getClientByNick(const std::string &nick);
		std::map<int, Client>::iterator getClientByUser(const std::string &user);
		const std::string getNick(int fd);
		const std::string getUser(int fd);
		const std::string getHost(int fd);
		int getFDByNick(const std::string &nick);
		int getFDByUser(const std::string &user);
		std::map<int, Client>::iterator begin(void);
		std::map<int, Client>::iterator end(void);

		// Setters
		t_numCode setNick(int fd, const std::string &newNick);
		t_numCode setUserInfo(int fd, const std::string &newUser,
			const std::string &realName);

		void add(Client &client);
		void add(int fd);
		void add(int fd, const std::string &host);
		void remove(int fd);
		void removeByNick(const std::string &nick);
		void removeByUser(const std::string &user);
		void removeClientFD(int clientFd);

		static bool isValidNick(const std::string &nick);
		static bool isValidUser(const std::string &user);
		static bool isSpecialChar(char ch);

	private:
		MsgHandler &_msgHandler;
		std::vector<struct pollfd>	&_pollFds;
		std::map<int, Client> _clients;
		std::map<std::string, std::map<int, Client>::iterator> _userToClient;
		std::map<std::string, std::map<int, Client>::iterator> _nickToClient;
};

#endif
