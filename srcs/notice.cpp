/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 12:55:28 by ychibani          #+#    #+#             */
/*   Updated: 2023/02/13 13:25:49 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_notice(std::string arg, User &user)
{
	std::stringstream	ss(arg);
	std::string			str;
	std::string			reply;

	ss >> str;

	reply = ":" + user.getNick() + "!" + user.getUser() + "@" + user.getIp() + " NOTICE ";
	reply += ss.str();
	reply += "\n";

	if (str[0] == '#')
		return 1;
	else
	{
		std::list<User>::iterator target_user = _getIteratorUser(str);
		if (target_user == _users.end())
			return 1;
		else
			target_user->send_to(reply, 0);
	}

	return 0;
}

