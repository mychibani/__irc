/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 18:17:23 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/14 14:52:16 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <sstream>
#include <cstdlib>
#include <list>
#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <csignal>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "User.hpp"
#include "Channel.hpp"

#define	PASS_ACCEPTED	"Wzds33kcdng78!as"
#define BUFFER_SIZE		2048

#define	RPL_WELCOME				001
#define RPL_YOURHOST			002
#define RPL_MYINFO				004
#define RPL_UMODEIS				221
#define RPL_CHANNELMODEIS		324
#define RPL_LUSERCLIENT			251
#define RPL_LUSEROP				252
#define RPL_LUSERUNKNOWN		253
#define RPL_LUSERCHANNELS		254
#define RPL_LUSERME				255
#define RPL_INFO				371
#define RPL_MOTD				372
#define RPL_MOTDSTART			375
#define RPL_ENDOFMOTD			376
#define ERR_NOSUCHCHANNEL		403
#define ERR_NOSUCHNICK			401
#define ERR_NORECIPIENT			411
#define ERR_NONICKNAMEGIVEN		431
#define ERR_ERRONEUSNICKNAME	432
#define ERR_NICKNAMEINUSE		433
#define ERR_NOTONCHANNEL		442
#define ERR_NOLOGIN				444
#define ERR_NEEDMOREPARAMS		461
#define ERR_ALREADYREGISTRED	462
#define ERR_PASSWDMISMATCH		464
#define ERR_NOTONCHANNEL		442
#define ERR_NOSUCHNICK			401
#define ERR_CHANOPRIVSNEEDED	482
#define	ERR_BADCHANMASK			476
#define ERR_CANNOTSENDTOCHAN	404
#define ERR_USERONCHANNEL		443
#define PONG					1000
#define TOPIC					1001

class User;
class Channel;

class Server
{
	public:
		Server(int port, std::string password, std::string servername);
		~Server(void);

		int		init(void);	
		int		loop(void);

	private:
		std::vector<pollfd>		_fds;
		std::list<User>			_users;
		std::list<Channel>		_channels;

		const int				_port;
		const std::string		_password;
		const std::string		_servername;

		int						_socket_fd;
		sockaddr_in				_socket_address;

		//parsing functions
		int		_isEmpty(const std::string &buffer);


		//Utils methods
		int		_error(const std::string &str);
		int		_is_a_nickname(const std::string &nickname) const;
		int		_nick_form(const std::string &nickname) const;
		int		_channel_form(const std::string &channel_name) const;
		void	_display_channels(void);
		int		_operators_online(void) const;
		int								_is_a_channel(const std::string &name) const;
		std::list<Channel>::iterator	_getIteratorChannel(const std::string &name);
		std::list<User>::iterator		_getIteratorUser(const std::string &name);

		//Private functionning methods
		int			_accept_user(void);
		void		_disconnect_user(std::list<User>::iterator &it_users, std::vector<pollfd>::iterator &it_fds);
		std::string	_command(int code, const User &user);
		std::string	_command(int code, const User &user, const Channel &chan);
		std::string	_command(int code, const User &user, const std::string &str);

		//IRC _commands
		int						_pass(std::string arg, User &user);
		int						_nick(std::string arg, User &user);
		int						_user(std::string arg, User &user);
		int						_join(std::string arg, User &user);
		int						_part(std::string arg, User &user);
		int						_mode(std::string arg, User &user);
		int						_topic(std::string arg, User &user);
		int						_kick(std::string arg, User &user);
		int						_invite(std::string arg, User &user);
		int						_quit(std::string arg, User &user);
		int						_privmsg(std::string arg, User &user);
		int						_ping(User &user);
		int						_info(User &user);
		int						_motd(User &user);
		int						_notice(std::string arg, User &user);

		//Impossible to copy it, assign it or default construct it
		Server(void);
		Server(const Server &model);
		Server	&operator=(const Server &model);
};

#endif
