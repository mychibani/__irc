/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascotto- <ascotto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 11:56:56 by ascotto-          #+#    #+#             */
/*   Updated: 2023/01/26 16:23:31 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Server.hpp"

int	ft_check_args(int argc, char **argv)
{
	if (argc != 3)
		return 1;
	for (int i = 0; argv[1][i]; i++)
		if (std::isdigit(argv[1][i]) == 0)
			return 1;

	return 0;
}

int main(int argc, char **argv)
{
	if (ft_check_args(argc, argv))
	{
		std::cout << "Wrong arguments. Usage:\n./ircserv <port> <password>" << std::endl;
		return 1;
	}

	Server	server(atoi(argv[1]), argv[2], "ServSSBU");

	if (server.init())
		return 1;
	
	if (server.loop())
		return 1;

	return 0;
}
