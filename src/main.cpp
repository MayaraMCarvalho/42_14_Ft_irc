/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macarval <macarval@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 16:51:56 by macarval          #+#    #+#             */
/*   Updated: 2024/08/22 20:45:11 by macarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <cstdlib>
#include <iostream>
#include "Colors.hpp"
#include "ConsoleLogger.hpp"
#include "IrcServer.hpp"


bool containsOnlyDigits(const std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::isdigit(str[i]))
		{
			return false;
		}
	}
	return true;
}

bool validateArguments(int argc, char *argv[], std::string &port, std::string &password)
{
	int portNum;
	std::string portStr;

	if (argc != 3)
	{
		std::cerr << RED;
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		std::cout << RESET << std::endl;
		return (false);
	}

	port = argv[1];
	portStr = argv[1];
	// Checks if the port is a valid integer
	if (!containsOnlyDigits(portStr))
	{

		std::cerr << "Error: Invalid port number: " << argv[1] << std::endl;
		return (false);
	} else {
		std::istringstream iss(portStr);
		iss >> portNum;
		// Additional range check for port number
		if (portNum <= 1024 || portNum > 65535)
		{
			std::cerr << "Error: Port number out of range" << std::endl;
			return (false);
		}
	}

	// The password is valid if it is not an empty string
	password = argv[2];
	if (password.empty())
	{
		std::cerr << RED;
		std::cerr << "Invalid password! "<< YELLOW << argv[2] << std::endl;
		std::cout << RESET << std::endl;
		return (false);
	}

	return (true);
}


int main(int argc, char *argv[])
{
	std::string	port;
	std::string	password;
	ConsoleLogger logger(Logger::DEBUG);

	if (!validateArguments(argc, argv, port, password))
		return (1);

	try
	{
		IRCServer server(port, password, logger);
		server.run();
	}
	catch (const std::exception &e)
	{
		logger.fatal("Exception caught: " + RED + e.what() + RESET);
		return (1);
	}
	return (0);
}
