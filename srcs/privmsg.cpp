/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/13 13:22:11 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_privmsg(std::string arg, User &user)
{
	std::stringstream	ss(arg);

	std::string			str;
	std::string			reply;

	ss >> str;

	reply = ":" + user.getNick() + "!" + user.getUser() + "@" + user.getIp() + " PRIVMSG ";
	reply += ss.str();
	reply += "\n";

	if (arg.empty())
	 	user.send_to(_command(ERR_NORECIPIENT, user), 0);
	if (str[0] == '#')
	{
		std::list<Channel>::iterator chan = this->_getIteratorChannel(str);

		if (chan == _channels.end())
			user.send_to(_command(ERR_CANNOTSENDTOCHAN, user, str), 0);
		else if (chan->hasMode('n') && chan->is_in_channel(user) == 0)
			user.send_to(_command(ERR_CANNOTSENDTOCHAN, user, chan->getName()), 0);
		else
			chan->send_to_channel(user, reply, 0);
	}
	else
	{
		std::list<User>::iterator target_user = this->_getIteratorUser(str);

		if (target_user == _users.end())
			user.send_to(_command(ERR_NOSUCHNICK, user, str), 0);
		else
			target_user->send_to(reply, 0);
	}

	return 0;
}
