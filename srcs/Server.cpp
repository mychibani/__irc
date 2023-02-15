/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:57:38 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/14 14:52:22 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	run = false;

void	handler(int)
{
	run = false;
}

Server::Server(int port, std::string password, std::string servername) : _port(port), _password(password), _servername(servername)
{
}

Server::~Server(void)
{

	for (std::list<User>::iterator it_fds = _users.begin(); it_fds != _users.end(); it_fds++)
		close(it_fds->getFd());
	close(_socket_fd);
}

int	Server::init(void)
{
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd == -1)
		return (this->_error("Socket error: "));

	int opt_val = 1;
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(int)) < 0)
    	this->_error("setsockopt(SO_REUSEADDR) failed");

	_socket_address.sin_family = AF_INET; 
	_socket_address.sin_addr.s_addr = INADDR_ANY;
	_socket_address.sin_port = htons(_port);

	if (bind(_socket_fd, reinterpret_cast<sockaddr *>(&_socket_address), sizeof(_socket_address)) != 0)
		return (close(_socket_fd), this->_error("Bind error: "));

	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) == -1)
		return (close(_socket_fd), this->_error("Flags set-up error: "));

	if (listen(_socket_fd, 5))
		return (close(_socket_fd), this->_error("Listen error: "));
		
	pollfd			new_pollfd;
	new_pollfd.fd = _socket_fd;
	new_pollfd.events = POLLIN;
	new_pollfd.revents = 0;

	_fds.push_back(new_pollfd);
	return 0;
}

int Server::loop(void)
{
	run = true;

	while (run)
	{
		if (signal(SIGINT, handler) == SIG_ERR)
			return (1);
		if (poll(_fds.data(), _fds.size(), -1) > 0)
		{
			if (_fds[0].revents == POLLIN)
				if (_accept_user())
					return 1;
			std::list<User>::iterator	it_users = _users.begin();
			for (std::vector<pollfd>::iterator it_fds = _fds.begin(); it_fds != _fds.end(); it_fds++)
			{
				if (it_fds == _fds.begin())
					it_fds++;
				if (it_fds->revents == POLLIN)
				{
					char				buffer[BUFFER_SIZE + 1];
					std::stringstream	s_stream;


					int bytes_received = recv(it_users->getFd(), &buffer, BUFFER_SIZE, 0);
					if (bytes_received <= 0)
					{
						it_users->setStatus(TO_DISCONNECT);
						this->_disconnect_user(it_users, it_fds);
						break ;
					}

					buffer[bytes_received] = '\0';
					s_stream << (it_users->getBuffer() + buffer);
					std::string	line;
					std::string	cmd;
					std::string arg;
#ifdef DEBUG
					std::cout << it_users->getFd() << " < " << buffer << std::endl;
#endif

					while (std::getline(s_stream, line, '\r'))
					{
						if (s_stream.rdstate() == std::ios_base::eofbit)
						{
							it_users->setBuffer(line);
							break ;
						}
						else
							it_users->setBuffer("");

						size_t	space_pos = line.find(' ');
						if (space_pos == std::string::npos)
						{
							cmd = line;
							arg = "";
						}
						else
						{
							cmd = line.substr(0, space_pos);
							arg = line.substr(space_pos + 1);
						}
	
						s_stream.seekg(static_cast<int>(s_stream.tellg()) + 1);
						
						
						if (cmd == "PASS")
							_pass(arg, *it_users);
						else if (cmd == "NICK")
						{
							if (_nick(arg, *it_users))
							{
								this->_disconnect_user(it_users, it_fds);
								break ;
							}
						}
						else if (cmd == "USER")
						{
							if (_user(arg, *it_users))
							{
								this->_disconnect_user(it_users, it_fds);
								break ;
							}

						}
						else if (cmd == "JOIN")
							_join(arg, *it_users);
						else if (cmd == "PART")
						{
							_part(arg, *it_users);
						}
						else if (cmd == "MODE")
						{
							_mode(arg, *it_users);
						}
						else if (cmd == "TOPIC")
						{
							_topic(arg, *it_users);
						}
						else if (cmd == "KICK")
						{
							_kick(arg, *it_users);
						}
						else if (cmd == "INVITE")
						{
							_invite(arg, *it_users);
						}

						else if (cmd == "QUIT")
						{
							_quit(arg, *it_users);
							this->_disconnect_user(it_users, it_fds);
							break ;
						}
						else if (cmd == "PRIVMSG")	
							_privmsg(arg, *it_users);
						else if (cmd == "PING")
							_ping(*it_users);
						else if (cmd == "info")
							_info(*it_users);
						else if (cmd == "motd")
							_motd(*it_users);
						else if (cmd == "NOTICE")	
							_notice(arg, *it_users);
						else
						{
							std::cout << "Unknown cmd: " << cmd << std::endl;
						}
					}
				}
			it_users++;
			}
		}
	}

	return 0;
}
