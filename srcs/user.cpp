/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/11 19:55:22 by ychibani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_user(std::string arg, User &user)
{
	std::string 		str;
	std::stringstream	ss(arg);

	if (user.getStatus() == CONNECTED)
		user.send_to(_command(ERR_ALREADYREGISTRED, user), 0);
	else
	{
		ss >> str;
		user.setUser(str);
		ss >> str;
		user.setHost(str);
		ss >> str;
		user.setIp(str);
		ss >> str;
		user.setName(str);
		if (ss.fail())
		{
			user.send_to(_command(ERR_NEEDMOREPARAMS, user, str), 0);
			user.setStatus(TO_DISCONNECT);
			return 1;
		}
		if (user.getPass() != PASS_ACCEPTED || (user.getPass() == "NO PASSWORD" && user.getPass() != PASS_ACCEPTED))
		{
			user.send_to(_command(ERR_PASSWDMISMATCH, user), 0);
			user.setStatus(TO_DISCONNECT);
			return 1;
		}
		if (user.getNick() != PASS_ACCEPTED)
		{
			user.send_to(": NICK :" + user.getNick() + "\n", 0);
			user.send_to(_command(RPL_WELCOME, user), 0);
			user.send_to(_command(RPL_YOURHOST, user), 0);
			user.send_to(_command(RPL_MYINFO, user), 0);
			user.send_to(_command(RPL_LUSERCLIENT, user), 0);
			user.send_to(_command(RPL_LUSEROP, user), 0);
			user.send_to(_command(RPL_LUSERUNKNOWN, user), 0);
			user.send_to(_command(RPL_LUSERME, user), 0);
			user.send_to(_command(RPL_MOTDSTART, user), 0);
			user.send_to(_command(RPL_MOTD, user), 0);
			user.send_to(_command(RPL_ENDOFMOTD, user), 0);
			user.setStatus(CONNECTED);
		}
	}

	return 0;
}
