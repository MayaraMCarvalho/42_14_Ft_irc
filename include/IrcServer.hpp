/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucperei <lucperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 13:40:10 by macarval          #+#    #+#             */
/*   Updated: 2024/07/17 15:13:34 by lucperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <poll.h>
#include "./client/ClientList.hpp"
#include "./channel/ChannelList.hpp"
#include "./bonus/FileTransfer.hpp"
#include "./bonus/Bot.hpp"
#include "Colors.hpp"
#include "./MsgHandler.hpp"
#include "Codes.hpp"

class	Channel;

class IRCServer
{
	private:
		std::string					_port;
		std::string					_password;
		int 						_serverFd;
		std::vector<struct pollfd>	_pollFds;
		FileTransfer 				*_fileTransfer;
		Bot 						_bot;
		MsgHandler					_msgHandler;
		ClientList					_clients;
		ChannelList					_channels;

		bool						_shouldExit;
		static IRCServer*			_instance;

		void		handleFileTransfer(int clientFd, const std::string &command);

	public:
	// Constructor & Destructor ===============================================
	
		IRCServer(const std::string &port, const std::string &password);
		~IRCServer( void );

	// Exceptions =============================================================

	// Getters ================================================================
	
		const		std::string& getPort(void);
		const		std::string& getPassword(void);
		int			getServerFd(void);
		const		std::vector<struct pollfd>& getPollFds(void);
		const		FileTransfer& getFileTransfer(void);
		const		Bot& getBot(void);
		//const		ClientList& getClients(void);
		//const		ChannelList& getChannels(void);

		ClientList	&getClients( void );
		ChannelList	&getChannels( void );
		MsgHandler	&getMsgHandler(void);

	// Setters ================================================================
	
		void		setPort(const std::string& port);
		void		setPassword(const std::string& password);
		void		setServerFd(int serverFd);
		void		setPollFds(const std::vector<struct pollfd>& pollFds);
		void		setFileTransfer(FileTransfer& fileTransfer);
		void		setBot(const Bot& bot);
		void		setClients(ClientList& clients);
		void		setChannels(ChannelList& channels);

	// Methods ================================================================

		void		setupServer(void);
		void		run(void);
		t_numCode	authenticate(int userFD, std::string password);
		void		acceptNewClient(void);
		void		handleClientMessage(int clientFd);
		void		removeClient(int clientFd);
		static void	signalHandler(int signal);
		static void	sendMessage(int clientFd, const std::string &message);
		void		setupSignalHandlers(void);
		static std::string	getHostName(const char *ip, const char *port);
};

#endif
