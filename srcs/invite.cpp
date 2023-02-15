/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascotto- <ascotto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/10 16:04:32 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_invite(std::string arg, User &user)
{
	std::stringstream	ss(arg);
	std::string			str;

	ss >> str;

	std::list<User>::iterator target_user = this->_getIteratorUser(str);

	if (target_user != _users.end())
	{
		ss >> str;

		if (str.length() >= 2 && str[0] == '#')
		{
			std::list<Channel>::iterator chan = this->_getIteratorChannel(str);

			if (chan == _channels.end())
				user.send_to(_command(ERR_NOSUCHNICK, user, str), 0);
			else
			{
				std::string	id = ":" + user.getNick() + "!" + user.getUser() + "@" + user.getIp();
				std::string rpl = id + " 341 " + user.getNick() + " " + target_user->getNick() + " " + chan->getName() + "\n";

				if (!chan->is_in_channel(user))
					user.send_to(_command(ERR_NOTONCHANNEL, user), 0);
				else if (!chan->is_operator(user))
					user.send_to(_command(ERR_CHANOPRIVSNEEDED, user, *chan), 0);
				else if (chan->is_in_channel(user) && chan->is_operator(user))
				{
					if (!chan->is_in_channel(*target_user))
					{
						if (chan->invite_user(*target_user) == 0)
						{
							user.send_to(rpl, 0);
							target_user->send_to((id + " INVITE " + target_user->getNick() + " " + chan->getName() + "\n"), 0);
						}
					}
					else
						user.send_to(_command(ERR_USERONCHANNEL, user, (target_user->getName() + " " + chan->getName())), 0);
				}
				return 0;
			}
		}
		else if (ss.fail())
			user.send_to(_command(ERR_NEEDMOREPARAMS, user), 0);
		else
			user.send_to(_command(ERR_NOSUCHNICK, user, str), 0);
	}
	else
		user.send_to(_command(ERR_NOSUCHNICK, user, str), 0);

	return 1;
}
