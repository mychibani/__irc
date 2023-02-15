/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascotto- <ascotto-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 18:17:23 by ascotto-          #+#    #+#             */
/*   Updated: 2023/02/13 15:35:42 by ascotto-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include <list>
#include <vector>

class User;

class Channel
{
	public:
		Channel(void);
		Channel(const std::string &name);
		Channel(const Channel &model);
		Channel	&operator=(const Channel &model);
		~Channel(void);

		const std::string	&getTopic(void) const;
		const std::string	&getName(void) const;
		const std::string	&getModes(void) const;

		int					getSize(void) const;

		void	setTopic(const std::string &topic);
		void	setName(const std::string &name);

		void	connect_user(User &user);
		int		disconnect_user(User &user);

		int		send_to_channel(User &user, std::string buffer, int flags);
		int		is_in_channel(const User &user) const;
		int		is_operator(const User &user) const;
		int		operators_connected_to_chan(void) const;

		int		is_invited(const User &user) const;
		int		invite_user(User &user);

		int		promote_to_op(User &user);
		int		deop(User &user);

		//MODE FUNCTIONS
		int		hasMode(const char c) const;
		void	addMode(const char c);
		void	delMode(const char c);

		//USELESS FUNCTION
		void	display_users(void);

	private:
		std::list<User *>			_users;
		std::list<User *>			_operators;
		std::list<User *>			_invited_users;

		std::string				_topic;
		std::string				_name;
		std::string				_modes;

		void	_send_name_reply(User &user);
};

#endif
