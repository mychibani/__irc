/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascotto- <ascotto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/10 14:45:20 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_join(std::string arg, User &user)
{
	std::stringstream	s_stream(arg);
	std::string			tmp;

	s_stream >> tmp;
	if (!this->_is_a_channel(tmp))
	{
		Channel	new_channel;

		new_channel.setName(tmp);
		new_channel.addMode('n');
		if (tmp[0] != '#' || this->_channel_form(tmp))
			user.send_to(_command(ERR_BADCHANMASK, user, new_channel), 0);
		else
			_channels.push_back(new_channel);
	}

	std::list<Channel>::iterator chan = this->_getIteratorChannel(tmp);

	if (chan != _channels.end())
		user.joinChannel(*chan);

	return 0;
}
