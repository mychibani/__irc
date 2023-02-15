/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascotto- <ascotto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/09 13:02:58 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_pass(std::string arg, User &user)
{
	if (arg.size() == 0)
		user.set_pass("NO PASSWORD");
	if (user.getStatus() == CONNECTED)
		user.send_to(_command(ERR_ALREADYREGISTRED, user), 0);
	else if (arg == _password)
	{
		user.set_pass(PASS_ACCEPTED);
		user.setNick(PASS_ACCEPTED);
	}

	return 0;
}
