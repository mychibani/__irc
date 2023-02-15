/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascotto- <ascotto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/13 17:29:23 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_kick(std::string arg, User &user)
{
	std::stringstream	ss(arg);
	std::string			str;

	ss >> str;

	if (str.length() >= 2 && str[0] == '#')
	{
		std::list<Channel>::iterator chan = this->_getIteratorChannel(str);
		if (chan == _channels.end())
			user.send_to(_command(ERR_NOSUCHCHANNEL, user, str), 0);
		else
		{
			ss >> str;

			if (ss.fail())
				user.send_to(_command(ERR_NEEDMOREPARAMS, user), 0);

			std::list<User>::iterator target_user = this->_getIteratorUser(str);

			if (target_user == _users.end())
				user.send_to(_command(ERR_NOTONCHANNEL, user, str), 0);
			if (&(*target_user) == &user)
				user.send_to(_command(ERR_NOTONCHANNEL, user, "Self-kicking is forbbiden"), 0);
			else 
			{
				if (chan->is_in_channel(user))
				{ 
					if (!chan->is_operator(user))
						user.send_to(_command(ERR_CHANOPRIVSNEEDED, user, *chan), 0);
					else if (!chan->is_in_channel(*target_user))
						user.send_to(_command(ERR_NOTONCHANNEL, user, (target_user->getName() + " " + chan->getName())), 0);
					else if (chan->is_in_channel(*target_user))
						target_user->leave_channel(*chan);
				}
				else
					user.send_to(_command(ERR_NOTONCHANNEL, user, *chan), 0);
			}
				
		}
	}
	else
		user.send_to(_command(ERR_NEEDMOREPARAMS, user), 0);

	return 0;
}
