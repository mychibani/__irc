/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/13 15:28:49 by ychibani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_part(std::string arg, User &user)
{
	std::stringstream	s_stream(arg);
	std::string			tmp;

	s_stream >> tmp;
	std::list<Channel>::iterator chan = this->_getIteratorChannel(tmp);
	if (chan != _channels.end())
	{
		if (!chan->is_in_channel(user))
		{
			user.send_to(_command(ERR_NOTONCHANNEL, user), 0);
			return 0;
		}
		user.leave_channel(*chan);
		if (chan->getSize() == 0)
			_channels.erase(chan);
	}
	else
		user.send_to(_command(ERR_NOSUCHCHANNEL, user), 0);

	return 0;
}
