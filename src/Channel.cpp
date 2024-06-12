/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucperei <lucperei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 18:17:28 by lucperei          #+#    #+#             */
/*   Updated: 2024/05/25 21:20:27 by lucperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"
#include "../include/IrcServer.hpp"

Channel::Channel(void) {}

Channel::Channel(const std::string &name) : _name(name) {}

void	Channel::add_client(int client_fd) {
	_clients.insert(client_fd);
}

void	Channel::remove_client(int client_fd) {
	_clients.erase(client_fd);
}

bool	Channel::is_client_in_channel(int client_fd) const {
	return (_clients.find(client_fd) != _clients.end());
}

// void	Channel::send_to_all(int except_fd) {
/*void	Channel::send_to_all(int except_fd, const std::string &message) {
    IRCServer *server = new IRCServer("port", "password");
	int	client_fd;
    for (std::set<int>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		client_fd = *it;
		if (client_fd != except_fd) {

			//send_message(client_fd, message);
			server->send_message(client_fd, message);		
		}
	}
    delete server;
}*/

// Precisa verificar por que ele n�o esta encaminhando as messagem para todos os clientes uma vez que estou especificando
void Channel::send_to_all(IRCServer *server, const std::string &message) {
    for (std::set<int>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        int client_fd = *it;
        server->send_message(client_fd, message);
    }
}