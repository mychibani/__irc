/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascotto- <ascotto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:57:38 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/13 16:58:23 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(void) : _nickname("Default"), _username("Default"), _hostname("Default"), _fd(-1)
{
	_connection_status = DISCONNECTED;
	_name = "Default";
	_ip = "Default";
	_modes = "";
	_buffer = "";
}

User::~User(void)
{
}

User::User(const User &model)
{
	*this = model;
}

User	&User::operator=(const User &model)
{
	if (this != &model)
	{
		_nickname = model._nickname;
		_username = model._username;
		_hostname = model._hostname;
		_buffer = model._buffer;
		_name = model._name;
		_ip = model._ip;
		_modes = model._modes;
		_fd = model._fd;
		_connection_status = model._connection_status;
	}
	return *this;
}

const std::string	&User::getNick(void) const
{
	return _nickname;
}

const std::string	&User::getUser(void) const
{
	return _username;
}

const std::string	&User::getHost(void) const
{
	return _hostname;
}

const std::string	&User::getBuffer(void) const
{
	return _buffer;
}

const std::string	&User::getName(void) const
{
	return _name;
}

const std::string	&User::getIp(void) const
{
	return _ip;
}

const std::string	&User::getModes(void) const
{
	return _modes;
}

const std::string	&User::getPass(void) const
{
	return _pass_accepted;
}

const int	&User::getFd(void) const
{
	return _fd;
}

void	User::setNick(const std::string &nickname)
{
	_nickname = nickname;
}

void	User::setUser(const std::string &username)
{
	_username = username;
}

void	User::setHost(const std::string &hostname)
{
	_hostname = hostname;
}

void	User::setBuffer(const std::string &buffer)
{
	_buffer = buffer;
}

void	User::setName(const std::string &name)
{
	_name = name;
}

void	User::setIp(const std::string &ip)
{
	_ip = ip;
}

void	User::setFd(const int fd)
{
	_fd = fd;
}

int	User::hasMode(const char c)
{
	for (int i = 0; _modes[i]; i++)
		if (_modes[i] == c)
			return 1;

	return 0;
}

void	User::addMode(const char c)
{
	if (c == 'i' || c == 'w' || c == 's' || c == 'o')
	{
		if (_modes.find(c) == std::string::npos)
			_modes += c;
	}
}

void	User::delMode(const char c)
{
	std::string flm;

	for (int i = 0; _modes[i]; i++)
	{
		if (_modes[i] != c)
			flm += _modes[i];
	}
	_modes = flm;
}

int	User::getStatus(void) const
{
	return _connection_status;
}

void	User::setStatus(int arg)
{
	_connection_status = arg;
}

void	User::set_pass(const std::string &pass_accepted)
{
	_pass_accepted = pass_accepted;
}

int	User::send_to(std::string buffer, int flags)
{
	int	ret;
	
#ifdef DEBUG
	std::cout << _fd << " > " << buffer;
#endif

	ret = send(_fd, buffer.c_str(), buffer.length(), flags);

	return ret;
}

void	User::leave_channel(Channel &channel_to_leave)
{
	channel_to_leave.disconnect_user(*this);
}

void	User::joinChannel(Channel &channel_to_join)
{
	std::string	str = ":" + _nickname + "!" + _username + "@" + _ip;
	str += " 473 " + _nickname + " " + channel_to_join.getName() + " :Cannot join channel (invite only)\n";

	if (channel_to_join.hasMode('i'))
	{
		if (channel_to_join.is_invited(*this) == 0)
			this->send_to(str, 0);
		else
			channel_to_join.connect_user(*this);
	}
	else
		channel_to_join.connect_user(*this);
}
