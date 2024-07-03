/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmachado <gmachado@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 13:40:10 by macarval          #+#    #+#             */
/*   Updated: 2024/07/03 03:38:06 by gmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include "ClientList.hpp"
#include "ChannelList.hpp"
#include "FileTransfer.hpp"
#include "Bot.hpp"
#include "MsgHandler.hpp"
#include "Codes.hpp"

class	Channel;

class IRCServer
{
	private:
		std::string					_port;
		std::string					_password;
		int 						_serverFd;
		std::vector<struct pollfd>	_pollFds;
		FileTransfer 				_fileTransfer;
		Bot 						_bot;
		MsgHandler					_msgHandler;
		ClientList					_clients;
		ChannelList					_channels;

		bool						_shouldExit;
		static IRCServer*			_instance;

		void handleFileTransfer(int clientFd, const std::string &command);

	public:
	// Constructor & Destructor ===============================================
		IRCServer(const std::string &port, const std::string &password);
		~IRCServer( void );

	// Exceptions =============================================================

	// Getters ================================================================
		ClientList			&getClients( void );
		ChannelList			&getChannels( void );
		const std::string	&getPassword( void );
		MsgHandler			&getMsgHandler(void);

	// Setters ================================================================

	// Methods ================================================================

		void				setupServer(void);
		void				run(void);
		t_numCode			authenticate(int userFD, std::string password);
		void				acceptNewClient(void);
		void				handleClientMessage(int clientFd);
		void				removeClient(int clientFd);
		static void			signalHandler(int signal);
		static void			setupSignalHandlers(void);
		static std::string	getHostName(const char *ip, const char *port);
};

#endif
