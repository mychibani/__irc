/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascotto- <ascotto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/14 14:48:32 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_mode(std::string arg, User &user)
{
	std::stringstream	ss(arg);
	std::string			str;

	ss >> str;
	if (str.length() >= 2 && str[0] == '#')
	{
		std::list<Channel>::iterator chan = this->_getIteratorChannel(str);

		if (chan != _channels.end())
		{
			std::string	target;
			std::string	mode;

			ss >> mode;
			ss >> target;

			if (target.empty() == 0)
			{
				std::list<User>::iterator target_user = this->_getIteratorUser(target);

				if (target_user == _users.end())
					user.send_to(_command(ERR_NOSUCHNICK, user, target), 0);
				else if (!(chan->is_in_channel(user) && chan->is_operator(user)))
					user.send_to(_command(ERR_CHANOPRIVSNEEDED, user, *chan), 0);
				else if (mode == "+o")
				{
					if (chan->is_in_channel(*target_user))
					{
						if (!(chan->is_operator(*target_user)))
							chan->promote_to_op(*target_user);
						target_user->send_to(_command(RPL_UMODEIS, *target_user), 0);
					}
					else
						user.send_to(_command(ERR_NOTONCHANNEL, user), 0);
				}
				else if (mode == "-o")
				{
					if (chan->is_in_channel(*target_user))
					{
						if (chan->is_operator(*target_user))
							chan->deop(*target_user);
						target_user->send_to(_command(RPL_UMODEIS, *target_user), 0);
					}
					else
						user.send_to(_command(ERR_NOTONCHANNEL, user), 0);
				}
			}
			else if (target.empty() == 1 && mode.empty() == 0)
			{
				if (!(chan->is_in_channel(user)))
						user.send_to(_command(ERR_NOTONCHANNEL, user), 0);
				else if (!(chan->is_operator(user)))
					user.send_to(_command(ERR_CHANOPRIVSNEEDED, user, *chan), 0);
				else if (!mode.empty() && mode[0] == '+')
				{
					for (int i = 1; mode[i]; i++)
						if (target.empty())
							chan->addMode(mode[i]);
				}
				else if (!mode.empty() && mode[0] == '-')
				{
					for (int i = 1; mode[i]; i++)
						if (target.empty())
							chan->delMode(mode[i]);
				}
			}
			user.send_to(_command(RPL_CHANNELMODEIS, user, *chan), 0);
		}
		else
			user.send_to(_command(ERR_NOSUCHCHANNEL, user, str), 0);

	}
	else if (str == user.getNick())
	{
		ss >> str;

		if (str.length() == 2 && str[0] == '+')
		{
			user.addMode(str[1]);
			user.send_to(_command(RPL_UMODEIS, user), 0);
		}
		else if (str.length() == 2 && str[0] == '-')
		{
			user.delMode(str[1]);
			user.send_to(_command(RPL_UMODEIS, user), 0);
		}
		else
			user.send_to(_command(ERR_NEEDMOREPARAMS, user), 0);
	}
	else
		user.send_to(_command(ERR_NOSUCHNICK, user, str), 0);

	return 0;
}
