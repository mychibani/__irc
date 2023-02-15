/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 13:44:35 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/13 17:30:44 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string	Server::_command(int code, const User &user)
{
	std::string	id;
	std::string	reply;

	id = user.getNick() + "!" + user.getUser() + "@" + user.getIp();

	switch (code)
	{
		case RPL_WELCOME:
		{
			reply = ":" + id + " 001 " + user.getNick();
			reply += " :Welcome to the Internet Relay Network ";
			reply += id;
			reply += "\n";
			return reply;
		}
		case RPL_YOURHOST:
		{
			reply = ":" + id + " 002 " + user.getNick();
			reply += " :Your host is localhost, running version 1.0\n";
			return reply;
		}
		case RPL_MYINFO:
		{
			reply = ":" + id + " 004 " + user.getNick();
			reply += " " + _servername + " 1.0 iwso ntio\n";
			return reply;
		}
		case RPL_UMODEIS:
		{
			reply = ":" + id + " 221 " + user.getNick();
			reply += " +" + user.getModes() + "\n";
			return reply;
		}
		case RPL_LUSERCLIENT:
		{
			std::stringstream ss;

			ss << (_fds.size() - 1);
			reply = ":" + id + " 251 " + user.getNick();
			reply += " :There are " + ss.str() + " user(s) on 1 server\n";
			return reply;
		}
		case RPL_LUSEROP:
		{
			std::stringstream	ss;

			ss << this->_operators_online();

			reply = ":" + id + " 252 " + user.getNick();
			reply += " " + ss.str() + " :operator(s) online\n";
			return reply;
		}
		case RPL_LUSERUNKNOWN:
		{
			reply = ":" + id + " 253 " + user.getNick();
			reply += " 0: unknown connection\n";
			return reply;
		}
		case RPL_LUSERCHANNELS:
		{
			std::stringstream ss;

			ss << (_channels.size());
			reply = ":" + id + " 254 " + user.getNick();
			reply += " " + ss.str() + " :channels formed\n";
			return reply;
		}
		case RPL_LUSERME:
		{
			std::stringstream ss;

			ss << (_fds.size() - 1);
			reply = ":" + id + " 255 " + user.getNick();
			reply += " :I have " + ss.str() + " clients and 0 servers\n";
			return reply;
		}
		case RPL_INFO:
		{
			reply = ":" + id + " 371 " + user.getNick();
			reply += " :SERVER INFOS \n";
			reply += ":" + id + " 371 " + user.getNick();
			reply += " :Server name: " + _servername + "\n";
			reply += ":" + id + " 371 " + user.getNick();
			reply += " :This server is a project for 42Paris school\n";
			reply += ":" + id + " 371 " + user.getNick();
			reply += " :Made by :\n";
			reply += ":" + id + " 371 " + user.getNick();
			reply += " :ascotto-\n";
			reply += ":" + id + " 371 " + user.getNick();
			reply += " :jroux-fo\n";
			reply += ":" + id + " 371 " + user.getNick();
			reply += " :END OF INFOS \n";
			return reply;
		}
		case RPL_MOTD:
		{
			reply = ":" + id + " 372 " + user.getNick();
			reply += " :- This server is super cool but don't forget guys...\n";
			reply += ":" + id + " 372 " + user.getNick();
			reply += " :- Mkleo is the best player in the world !\n";
			return reply;
		}
		case RPL_MOTDSTART:
		{
			reply = ":" + id + " 375 " + user.getNick();
			reply += " :" + _servername + " Message of the day\n";
			return reply;
		}
		case RPL_ENDOFMOTD:
		{
			reply = ":" + id + " 376 " + user.getNick();
			reply += " :End of /MOTD command \n";
			return reply;
		}
		case ERR_NOSUCHCHANNEL:
		{
			reply = ":" + id + " 403 " + user.getNick();
			reply += " :No such channel\n";
			return reply;
		}
		case ERR_NONICKNAMEGIVEN:
		{
			reply += " :No nickname given.\n";
			return reply;
		}
		case ERR_ERRONEUSNICKNAME:
		{
			reply = ":" + id + " 432";
			reply += " :Erroneus nickname.\n";
			return reply;
		}
		case ERR_NICKNAMEINUSE:
		{
			reply = ":" + id + " 433 " + user.getNick();
			reply += " :This nickname is already in use.\n";
			return reply;
		}
		case ERR_NOLOGIN:
		{
			reply = ":" + id + " 444 " + user.getNick();
			reply += " :This user isn't logged.\n";
			return reply;
		}
		case ERR_NEEDMOREPARAMS:
		{
			reply = ":" + id + " 461 " + user.getNick();
			reply += " :Need more params.\n";
			return reply;
		}
		case ERR_ALREADYREGISTRED:
		{
			reply = ":" + id + " 462 " + user.getNick();
			reply += " :User is already registered.\n";
			return reply;
		}
		case ERR_PASSWDMISMATCH:
		{
			reply = ":" + id + " 464 " + user.getNick();
			reply += " :Password is incorrect.\n";
			return reply;
		}
		case PONG:
		{
			reply = ":" + id;
			reply += " PONG :" + user.getNick() + "\n";
			return reply;
		}
	}
	return "FAILURE";
}

std::string	Server::_command(int code, const User &user, const Channel &chan)
{
	std::string	id;
	std::string	reply;

	id = user.getNick() + "!" + user.getUser() + "@" + user.getIp();

	switch (code)
	{
		case RPL_CHANNELMODEIS:
		{
			reply = ":" + id + " 324 " + user.getNick();
			reply += " " + chan.getName() + " +" + chan.getModes() + "\n";
			return reply;
		}
		case TOPIC:
		{
			reply = ":" + id + " TOPIC " + chan.getName();
			reply += " :" + chan.getTopic() + "\n";
			return reply;
		}
		case ERR_CHANOPRIVSNEEDED:
		{
			reply = ":" + id + " 482 " + user.getNick();
			reply += " " + chan.getName() + " :You're not channel operator.\n";
			return reply;
		}
		case ERR_NOTONCHANNEL:
		{
			reply = ":" + id + " 442 " + user.getNick();
			reply += " " + chan.getName() + " :You're not connected to channel.\n";
			return reply;
		}
		case ERR_BADCHANMASK:
		{
			reply = ":" + id + " 476 " + user.getNick();
			reply += " " + chan.getName() + " :Invalid channel name.\n";
			return reply;
		}
		case ERR_NORECIPIENT:
		{
			reply = ":" + id + " 411 " + user.getNick();
			reply += " " + chan.getName() + ":No recipient given";
			return reply;
		}
		case ERR_NOSUCHCHANNEL:
		{
			reply = ":" + id + " 401 " + user.getNick();
			reply += " " + chan.getName() + " :No such nick/channel";
			return reply;
		}
	}
	return "FAILURE";
}

std::string	Server::_command(int code, const User &user, const std::string &str)
{
	std::string	id;
	std::string	reply;

	id = user.getNick() + "!" + user.getUser() + "@" + user.getIp();

	switch (code)
	{
		case ERR_CANNOTSENDTOCHAN:
		{
			reply = ":" + id + " 404 " + user.getNick();
			reply += " " + str + " :Cannot send to channel.\n";
			return reply;
		}
		case ERR_NOSUCHNICK:
		{
			reply = ":" + id + " 401 " + user.getNick();
			reply += " " + str + " :No such nickname.\n";
			return reply;
		}
		case ERR_NOSUCHCHANNEL:
		{
			reply = ":" + id + " 403 " + user.getNick();
			reply += " " + str + " :No such channel.\n";
			return reply;
		}
		case ERR_ERRONEUSNICKNAME:
		{
			reply = ":" + id + " 432 " + user.getNick();
			reply += + " " + str + " :Erroneus nickname.\n";
			return reply;
		}
		case ERR_NICKNAMEINUSE:
		{
			reply = ":" + id + " 433 " + user.getNick();
			reply += " " + str + " :This nickname is already in use.\n";
			return reply;
		}
		case ERR_USERONCHANNEL:
		{
			reply = ":" + id + " 443 " + user.getNick();
			reply += " " + str + " :is already on channel.\n";
			return reply;
		}
		case ERR_NOTONCHANNEL:
		{
			reply = ":" + id + " 442 " + user.getNick();
			reply += " " + str + " :You're not connected to channel.\n";
			return reply;
		}
		case ERR_NEEDMOREPARAMS:
		{
			reply = ":" + id + " 461 " + user.getNick();
			reply += " " + str + " :Need more params.\n";
			return reply;
		}
	}
	return "FAILURE";
}
