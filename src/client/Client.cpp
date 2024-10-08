/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmachado <gmachado@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 03:43:45 by gmachado          #+#    #+#             */
/*   Updated: 2024/06/12 03:43:48 by gmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, MsgHandler &msgHandler) : _msgHandler(msgHandler),
	_nick(""), _user(""), _host(""), _realName(""), _fd(fd),
	_channels(), _modeFlags(Client::NO_MODE), _status(Client::CONNECTED) { }

Client::Client(const Client &src) : _msgHandler(src._msgHandler),
	_nick(src._nick), _user(src._user), _host(src._host),
	_realName(src._realName), _fd(src._fd), _channels(src._channels),
	_modeFlags(src._modeFlags), _status(src._status) { }


Client::~Client(void) { }

Client &Client::operator=(const Client &src) {
	if (this == &src)
		return *this;

	_nick = src._nick;
	_user = src._user;
	_host = src._host;
	_realName = src._realName;
	_fd = src._fd;
	_channels = src._channels;
	_modeFlags = src._modeFlags;
	_status = src._status;

	return *this;
}

// Getters
const std::string &Client::getNick(void) { return _nick; }

const std::string &Client::getUser(void) { return _user; }

const std::string &Client::getHost(void) { return _host; }

const std::string &Client::getRealName(void) { return _realName; }

const std::string Client::getFullId(void) {
	return _nick + '!' + _user + '@' + _host;
}

int Client::getFD(void) { return _fd; }

std::set<std::string> &Client::getChannelList(void) {
	return _channels;
}

Client::t_status Client::getStatus(void) { return _status; }

bool Client::getMode(t_mode mode) { return (_modeFlags & mode) != 0; }

int Client::getModeFlags(void) { return _modeFlags; }

// Setters

void Client::setNick(const std::string &nick) { _nick = nick; }

void Client::setHost(const std::string &host) { _host = host; }

void Client::setUser(const std::string &user) { _user = user; }

void Client::setRealName(const std::string &realName) { _realName = realName; }

void Client::setModeFlags(int modeFlags) { _modeFlags = modeFlags; }

void Client::setMode(const std::string &modeStr) {

	int newModeFlags;

	if (modeStr.length() != 2)
		return;

	switch (modeStr[1])
	{
		case 'a':
			newModeFlags = AWAY;
			break;
		case 'i':
			newModeFlags = INVISIBLE;
			break;
		case 'w':
			newModeFlags = WALLOPS;
			break;
		case 'r':
			newModeFlags = RESTRICTED;
			break;
		case 'o':
			newModeFlags = OPERATOR;
			break;
		case 'O':
			newModeFlags = LOCAL_OP;
			break;
		case 's':
			newModeFlags = RECV_NOTICES;
			break;
		default:
			return;
	}

	if (modeStr[0] == '+')
		_modeFlags |= newModeFlags;
	else if (modeStr[0] == '-')
		_modeFlags &= (~newModeFlags);
}
void Client::setStatus(t_status status) { _status = status; }

// Channel functions
bool Client::isInChannel(const std::string &channelStr) {
	return _channels.find(channelStr) != _channels.end();
}

void Client::addChannel(const std::string &channelStr)
{
	_channels.insert(channelStr);
}

void Client::removeChannel(const std::string &channelStr) {
	_channels.erase(channelStr);
}

void Client::sendMessage(const std::string &from, const std::string &msg) {
	_msgHandler.sendMessage(_fd, from, msg);
}

void Client::sendMessage(const std::string &msg) {
	_msgHandler.sendMessage(_fd, msg);
}
