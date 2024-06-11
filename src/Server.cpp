/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macarval <macarval@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:58:55 by macarval          #+#    #+#             */
/*   Updated: 2024/06/11 09:08:35 by macarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Constructor & Destructor ===================================================
Server::Server( void ) {}

Server::~Server( void ) {}

// Getters ====================================================================

// Setters ====================================================================

// Methods ====================================================================
void	Server::test( char const *argv[] )
{
	std::cout << YELLOW << "OK" << std::endl;
	std::cout << BLUE << argv[0] << std::endl;
	std::cout << argv[1] << std::endl;
	std::cout << argv[2] << std::endl;
}
// Exceptions =================================================================

