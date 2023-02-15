/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/11 20:54:41 by ychibani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_ping(User &user)
{
	if (user.getStatus() == CONNECTED)
		user.send_to(_command(PONG, user), 0 );
	else
		user.send_to(_command(ERR_NOLOGIN, user), 0);
	return 0;
}
