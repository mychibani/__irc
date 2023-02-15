/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/13 13:20:40 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_topic(std::string arg, User &user)
{
	std::stringstream	ss(arg);
	std::string			str;

	ss >> str;
	if (str.length() >= 2 && str[0] == '#')
	{
		std::list<Channel>::iterator chan = this->_getIteratorChannel(str);

		if (chan != _channels.end())
		{
			std::string	topic;

			ss >> topic;

			if (chan->hasMode('t') && !(chan->is_in_channel(user) && chan->is_operator(user)))
				user.send_to(_command(ERR_CHANOPRIVSNEEDED, user, *chan), 0);
			else if (!topic.empty() && topic[0] == ':' && chan->is_in_channel(user))
			{
				chan->setTopic(topic.substr(1));
				user.send_to(_command(TOPIC, user, *chan), 0);
				chan->send_to_channel(user, _command(TOPIC, user, *chan), 0);
			}
			else
				user.send_to(_command(ERR_NOTONCHANNEL, user, *chan), 0);
		}
		else
			user.send_to(_command(ERR_NOSUCHCHANNEL, user, str), 0);
	}
	else
		user.send_to(_command(ERR_NEEDMOREPARAMS, user), 0);
	return 0;
}
