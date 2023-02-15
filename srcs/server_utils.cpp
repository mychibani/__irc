/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascotto- <ascotto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:57:38 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/13 15:41:06 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	Server::_error(const std::string &str)
{
	std::cerr << str << std::strerror(errno) << std::endl;

	return -1;
}

int	Server::_is_a_channel(const std::string &name) const
{

	for (std::list<Channel>::const_iterator it = _channels.begin() ; it != _channels.end(); it++)
	{
		if (it->getName() == name)
			return 1;
	}

	return 0;
}

int	Server::_is_a_nickname(const std::string &nickname) const
{
	for (std::list<User>::const_iterator it = _users.begin() ; it != _users.end(); it++)
	{
		if (it->getNick() == nickname)
			return 1;
	}

	return 0;
}

int Server::_nick_form(const std::string &nickname) const
{
	if (nickname.size() > 15)
		return (1);
	if (nickname == "anonymous" || nickname == "")
		return (1);
	for (int i = 0; nickname[i]; i++)
	{
		if (nickname[i] > 126 || nickname[i] < 32)
			return (1);
		else if (isalnum(nickname[i]) == 0)
			if (nickname[i] != '_' && nickname[i] != '-')
				return (1);
	}
	return (0);
}

int Server::_channel_form(const std::string &channel_name) const
{
	if (channel_name.size() > 15)
		return (1);
	if (channel_name == "")
		return (1);
	for (int i = 1; channel_name[i]; i++)
	{
		if (isalnum(channel_name[i]) == 0)
			return (1);
	}
	return (0);
}

void	Server::_display_channels(void)
{
	for (std::list<Channel>::iterator it = _channels.begin() ; it != _channels.end(); it++)
	{
		std::cout << it->getName() << ": " << std::endl; 
		it->display_users();
	}
}

int	Server::_operators_online(void) const
{
	int	ret;

	ret = 0;
	for (std::list<Channel>::const_iterator it = _channels.begin() ; it != _channels.end(); it++)
		ret += it->operators_connected_to_chan();

	return ret;
}



std::list<Channel>::iterator Server::_getIteratorChannel(const std::string &name)
{
	for (std::list<Channel>::iterator it = _channels.begin() ; it != _channels.end(); it++)
	{
		if (it->getName() == name)
			return it;
	}

	return _channels.end();
}

std::list<User>::iterator Server::_getIteratorUser(const std::string &name)
{
	for (std::list<User>::iterator it = _users.begin() ; it != _users.end(); it++)
	{
		if (it->getNick() == name)
			return it;
	}

	return _users.end();
}

int	Server::_isEmpty(const std::string &buffer)
{
	int i = 0;

	while (buffer[i])
	{
		if (buffer[i] != '\0')
			return (1);
		i++;
	}
	return (0);
}

void Server::_disconnect_user(std::list<User>::iterator &it_users, std::vector<pollfd>::iterator &it_fds)
{
    std::cout << "Client " << it_users->getNick() << " disconnected." << std::endl;

	for (std::list<Channel>::iterator it = _channels.begin() ; it != _channels.end();)
	{
		it->disconnect_user(*it_users);
		if (it->getSize() == 0)
			it = _channels.erase(it);
		else
			it++;
	}

    it_fds = _fds.erase(it_fds);
    close(it_users->getFd());
    it_users = _users.erase(it_users);
    it_fds--;
    it_users--;
}

int Server::_accept_user(void)
{
	sockaddr_in	client_address;

	char ip[INET_ADDRSTRLEN];//for human readable ip
	socklen_t len = sizeof(client_address);

	int client_sockfd = accept(_socket_fd, reinterpret_cast<sockaddr *>(&client_address), &len);
	if (client_sockfd == -1)
		_error("Accept error: ");

	inet_ntop(AF_INET, &(client_address.sin_addr), ip, INET_ADDRSTRLEN);
	//mayb needs a if

	User	new_user;

	new_user.setHost(ip);
	new_user.setFd(client_sockfd);

	std::cout << "Client connected : " << new_user.getHost() << ":" << ntohs(client_address.sin_port) << std::endl;

	pollfd			new_pollfd;
	new_pollfd.fd = client_sockfd;
	new_pollfd.events = POLLIN;
	new_pollfd.revents = 0;

	_fds.push_back(new_pollfd);
	_users.push_back(new_user);

	return 0;
}
