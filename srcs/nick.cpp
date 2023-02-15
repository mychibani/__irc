/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascotto- <ascotto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/09 13:14:09 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_nick(std::string arg, User &user)
{
	std::string	id;

	id = ":" + user.getNick() + "!" + user.getUser() + "@" + user.getIp();

	if (user.getStatus() == CONNECTED)
	{

		if (this->_is_a_nickname(arg))
			user.send_to(_command(ERR_NICKNAMEINUSE, user, arg), 0);
			// user.send_to((id + " 433 " + user.getNick() + " :" + arg + "\n"), 0);
		else if (this->_nick_form(arg))
		{
			user.send_to(_command(ERR_ERRONEUSNICKNAME, user, arg), 0);
			// user.setStatus(TO_DISCONNECT);
			// return 1;
		}
		else if (arg.empty())
		{
			user.send_to(_command(ERR_NONICKNAMEGIVEN, user), 0);
			// user.setStatus(TO_DISCONNECT);
			// return 1;
		}
		else
		{
			user.setNick(arg);
			user.send_to((id + " NICK :" + user.getNick() + "\n"), 0);
		}
	}
	else
	{
		if (this->_is_a_nickname(arg))
		{
			user.send_to(_command(ERR_NICKNAMEINUSE, user, arg), 0);
			user.setStatus(TO_DISCONNECT);
			return 1;
		}
		else if (arg.empty())
		{
			user.send_to(_command(ERR_NONICKNAMEGIVEN, user), 0);
			user.setStatus(TO_DISCONNECT);
			return 1;
		}
		else if (this->_nick_form(arg))
		{
			user.send_to(_command(ERR_ERRONEUSNICKNAME, user, arg), 0);
			user.setStatus(TO_DISCONNECT);
			return 1;
		}
		else
			user.setNick(arg);
	}

	return 0;
}
