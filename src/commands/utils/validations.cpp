/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macarval <macarval@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 20:25:52 by macarval          #+#    #+#             */
/*   Updated: 2024/08/05 11:05:35 by macarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

bool Commands::initValidation(size_t numArgs)
{
	std::string	error;

	if (_args.size() < numArgs)
	{
		error = errorNeedMoreParams();
		if (_args[0] == NICK)
			error = errorNoNicknameGiven();
		else if (_args[0] == PRIVMSG && _args.size() == 2)
		{
			error = errorNoTextToSend();
			if (_args[1][0] == ':')
				error = errorNoRecipient();
		}
		printInfo(error);
	}
	else if (_args.size() > numArgs && _args[0] == NICK)
		printInfo(errorErroneusNickname(_args[1]));
	else
		return true;
	return false;
}

bool Commands::validSetup(void)
{
	int		status = _clients.getClient(_fd)->second.getStatus();

	bool	isUser = (_args[0] == USER);
	bool	isNick = (_args[0] == NICK);

	bool	isAuth = (status == Client::AUTHENTICATED);
	bool	isGotNick = (status == Client::GOT_NICK);
	bool	isGotUser = (status == Client::GOT_USER);
	bool	isReg = (status == Client::REGISTERED);

	if (!isReg && ((!isUser && !isNick)
		|| (!isAuth && ((isUser && !isGotNick) || (isNick && !isGotUser)))))
		printInfo(errorNotRegistered());
	else
		return true;
	return false;

}

bool Commands::validArg(std::string &arg)
{
	std::string	error;

	if (arg.empty())
		printInfo(errorNeedMoreParams());
	else if (arg.size() > MAX_LENGTH)
	{
		if (_args[0] == NICK)
			printInfo(errorErroneusNickname(arg));
		else
			printInfo(errorInputTooLong());
	}
	else if (invalidChar(arg))
	{
		if (_args[0] == NICK || _args[0] == USER)
			printInfo(errorErroneusNickname(arg));
		else
			printInfo(errorUnknownError());
	}
	else
		return true;
	return false;
}

bool Commands::invalidChar(std::string &arg)
{
	if (_args[0] != NICK && _args[0] != USER && _args[0] != QUIT
		&& (arg[0] == '#' || arg[0] == '&'))
		return false;
	if (arg.find_first_not_of(ALPHA_NUM_SET) != std::string::npos)
		return true;
	return false;
}

bool Commands::validChannelName(std::string &channel)
{
	if ((channel[0] == '#' || channel[0] == '&')
		&& channel.find_first_not_of(ALPHA_NUM_SET, 1) != std::string::npos)
		printInfo(errorBadChanMask(channel));
	else if (channel[0] != '#' && channel[0] != '&')
		printInfo(errorNoSuchChannel(channel));
	else
		return true;
	return false;
}

bool Commands::validMessage(std::string &message)
{
	std::string	error = "";

	if (message[0] != ':')
		printInfo(errorNeedMoreParams());
	else if (message == ":" && _args[0] != TOPIC)
	{
		if (_args[0] == USER)
			printInfo(errorNeedMoreParams());
		else
			printInfo(errorNoTextToSend());
	}
	else
	{
		message.erase(0, 1);
		return true;
	}
	return false;
}
