/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/13 15:28:38 by ychibani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_quit(std::string arg, User &user)
{
	std::string reply;

	reply = "QUIT :QUIT ";
	if (arg.empty())
		reply += "leaving\n";
	else
	{
		reply += arg;	
		reply += '\n';
	}
	user.send_to(reply, 0);
	user.setStatus(TO_DISCONNECT);
	return 0;
}
