/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascotto- <ascotto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 18:17:23 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/13 16:58:31 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Channel.hpp"
#include "Server.hpp"

class Channel;
class Server;

#define DISCONNECTED	0
#define	CONNECTED		1
#define TO_DISCONNECT	2

class User
{
	public:
		User(void);
		~User(void);
		User(const User &model);
		User	&operator=(const User &model);

		const std::string	&getNick(void) const;
		const std::string	&getUser(void) const;
		const std::string	&getHost(void) const;
		const std::string	&getBuffer(void) const;
		const std::string	&getName(void) const;
		const std::string	&getIp(void) const;
		const std::string	&getModes(void) const;
		const std::string	&getPass(void) const;
		const int			&getFd(void) const;
		int					getStatus(void) const;

		void	setNick(const std::string &nickname);
		void	setUser(const std::string &username);
		void	setHost(const std::string &hostname);
		void	setBuffer(const std::string &buffer);
		void	setName(const std::string &name);
		void	setIp(const std::string &ip);
		void	setFd(const int fd);
		void	setStatus(int arg);

		int		hasMode(const char c);
		void	addMode(const char c);
		void	delMode(const char c);


		void	set_pass(const std::string &pass_accepted);

		int		send_to(std::string buffer, int flags);

		void				leave_channel(Channel &channel_to_leave);
		void				joinChannel(Channel &channel_to_join); 

	private:
		std::string	_nickname;
		std::string	_username;
		std::string	_hostname;
		std::string	_name;
		std::string	_ip;

		std::string	_pass_accepted;

		std::string	_modes;

		std::string	_buffer;

		int		_fd;
		int		_connection_status;
};

#endif
