/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandSetup.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmachado <gmachado@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 16:51:34 by macarval          #+#    #+#             */
/*   Updated: 2024/08/22 03:04:22 by gmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void Commands::commandPass( void )
{
	Client	&client = _clients.getClient(_fd)->second;
	Client::t_status status = client.getStatus();

	if (_args.size() != 2)
		printInfo(errorNeedMoreParams());
	else if (status == Client::AUTHENTICATED || status == Client::GOT_NICK
			|| status == Client::GOT_USER || status == Client::REGISTERED)
		printInfo(errorAlredyRegister());
	else
	{
		std::string	pass = _args[1];

		if (pass.size() > MAX_LENGTH)
			printInfo(errorNeedMoreParams());
		else if (pass != _serverPass)
			printInfo(errorPassMismatch());
		else if (status == Client::CONNECTED)
			client.setStatus(Client::AUTHENTICATED);
	}
}

void Commands::commandNick( void )
{
	if (validSetup() && initValidation(2))
	{
		std::string	nick = _args[1];

		if (!validArg(nick))
			return ;

		std::map<int, Client>::iterator	exist = _clients.getClientByNick(nick);

		if (exist != _clients.end())
			printInfo(errorNicknameInUse(nick));
		else
			saveNick(nick);
	}
}

void Commands::saveNick(std::string &nick)
{
	t_numCode	errorCode = NO_CODE;
	Client &client = _clients.getClient(_fd)->second;

	errorCode = _clients.setNick(_fd, nick);

	int	status = client.getStatus();
	if (status == Client::REGISTERED)
	{
		printInfo(getWelcome(client));
		printInfo(getYourHost(client));
		printInfo(getMyInfo(client));
	}

	if (errorCode != NO_CODE)
		printInfo(toString(errorCode));
}

void Commands::commandUser( void )
{
	if (validSetup() && initValidation(5))
	{
		std::string	user = _args[1];
		std::string	realName = getMessage(4);

		if (!validArg(user) || !validMessage(realName))
			return ;

		int	status = _clients.getClient(_fd)->second.getStatus();
		if (status == Client::REGISTERED)
			printInfo(errorAlredyRegister());
		else if (status == Client::GOT_NICK || status == Client::AUTHENTICATED)
			saveUser(user, realName);
		else
			printInfo(errorNotRegistered());
	}
}

void Commands::saveUser(std::string &user, std::string &realName)
{
	t_numCode	errorCode = NO_CODE;
	Client &client = _clients.getClient(_fd)->second;

	errorCode = _clients.setUserInfo(_fd, user, realName);
	if (errorCode == NO_CODE)
	{
		int	status = client.getStatus();
		if (status == Client::REGISTERED)
		{
			printInfo(getWelcome(client));
			printInfo(getYourHost(client));
			printInfo(getMyInfo(client));
		}
	}
	else
		printInfo(toString(errorCode));
}
