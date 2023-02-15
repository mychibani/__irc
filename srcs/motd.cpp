/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:49:20 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/11 20:12:08 by ychibani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_motd(User &user)
{
	user.send_to(_command(RPL_MOTDSTART, user), 0);
	user.send_to(_command(RPL_MOTD, user), 0);
	user.send_to(_command(RPL_ENDOFMOTD, user), 0);

	return 0;
}
