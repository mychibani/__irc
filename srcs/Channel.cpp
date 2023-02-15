/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:57:38 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/13 15:37:13 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void) : _topic(""), _name("NewChan"), _modes("")
{
}

Channel::Channel(const std::string &name) : _topic(""), _name(name), _modes("")
{
}

Channel::~Channel(void)
{
}

Channel::Channel(const Channel &model) 
{
	*this = model;
}

Channel	&Channel::operator=(const Channel &model)
{
	if (this != &model)
	{
		_topic = model._topic;
		_name = model._name;
		_modes = model._modes;
		_users = model._users;
		_operators = model._operators;
		_invited_users = model._invited_users;
	}
	
	return *this;
}

const std::string	&Channel::getTopic(void) const
{
	return _topic;
}

const std::string	&Channel::getName(void) const
{
	return _name;
}

const std::string	&Channel::getModes(void) const
{
	return _modes;
}

int	Channel::getSize(void) const
{
	return _users.size();
}

void	Channel::setTopic(const std::string &topic)
{
	_topic = topic;
}

void	Channel::setName(const std::string &name)
{
	_name = name;
}

int	Channel::send_to_channel(User &user, std::string buffer, int flags)
{
	int	ret = 0;

	for (std::list<User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (*it != &user)
			ret = (*it)->send_to(buffer, flags);
		if (ret == -1)
			return ret;
	}

	return 0;
}

void Channel::_send_name_reply(User &user)
{
	std::string	id;
	std::string	str;

	id = ":" + user.getNick() + "!" + user.getUser() + "@" + user.getIp();

	str = id + " 353 " + user.getNick() + " = " + _name + " :";
	for (std::list<User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (is_operator(**it))
			str += "@";
		str += (*it)->getNick();
		str += " ";
	}
	str += "\n";

	if (_topic.empty() == 0)
		user.send_to((id + " 332 " + user.getNick() + " " + _name + " :" + _topic + "\n"), 0);
	user.send_to(str, 0);
	user.send_to((id + " 366 " + user.getNick() + " " + _name + " :End of /NAMES list\n"), 0);
	user.send_to((id + " JOIN :" + _name + "\n"), 0);
	this->send_to_channel(user, (id + " JOIN :" + _name + "\n"), 0);
}

void	Channel::connect_user(User &user)
{
	std::string	str;

	if (this->is_in_channel(user))
		return ;
	else
	{
		_users.push_back(&user);
		if (this->getSize() <= 1)
			_operators.push_back(&user);
		if (this->is_invited(user))
			_invited_users.remove(&user);
		this->_send_name_reply(user);
	}
}

int	Channel::disconnect_user(User &user)
{
	std::string	str;
	std::list<User *>::iterator it;

	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (*it == &user)
			break ;
	}

	if (it == _users.end())
		return 1;

	str = ":" + user.getNick() + "!" + user.getUser() + "@" + user.getIp() + " PART " + _name + "\n";

	user.send_to(str, 0);
	this->send_to_channel(user, str, 0);

	_users.remove(&user);
	if (is_operator(user))
		_operators.remove(&user);

	return 0;
}

int Channel::is_in_channel(const User &user) const
{
	for (std::list<User *>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (*it == &user)
			return 1;
	}

	return 0;
}

int Channel::is_operator(const User &user) const
{
	for (std::list<User *>::const_iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == &user)
			return 1;
	}

	return 0;
}

int	Channel::operators_connected_to_chan(void) const
{
	return	_operators.size();
}

int	Channel::hasMode(const char c) const
{
	for (int i = 0; _modes[i]; i++)
		if (_modes[i] == c)
			return 1;

	return 0;
}

void	Channel::addMode(const char c)
{
	if (c == 'n' || c == 't' || c == 'i' || c == 'o')
	{
		if (_modes.find(c) == std::string::npos)
			_modes += c;
	}
}

void	Channel::delMode(const char c)
{
	std::string flm;

	for (int i = 0; _modes[i]; i++)
	{
		if (_modes[i] != c)
			flm += _modes[i];
	}
	_modes = flm;
}

int	Channel::promote_to_op(User &user)
{
	if (is_operator(user) == 0)
	{
		_operators.push_back(&user);
		user.addMode('o');
	}
	else
		return 1;

	return 0;
}

int	Channel::deop(User &user)
{
	if (is_operator(user) == 1)
	{
		_operators.remove(&user);
		user.delMode('o');
	}
	else
		return 1;

	return 0;
}

int Channel::is_invited(const User &user) const
{
	for (std::list<User *>::const_iterator it = _invited_users.begin(); it != _invited_users.end(); it++)
	{
		if (*it == &user)
			return 1;
	}

	return 0;
}


int	Channel::invite_user(User &user)
{
	if (is_invited(user) == 0)
		_invited_users.push_back(&user);
	else
		return 1;

	return 0;
}

//USELESS
void	Channel::display_users(void)
{
	for (std::list<User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		std::cout << "\t- ";
		if (is_operator(**it))
			std::cout << "@";
		std::cout << (*it)->getNick() << std::endl; 
	}
	std::cout << std::endl; 
}
