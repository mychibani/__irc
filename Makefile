# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ychibani <ychibani@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/11 14:02:45 by ascotto-          #+#    #+#              #
#    Updated: 2023/02/13 15:51:12 by ascotto-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			=	c++
CXXFLAGS	=	-MD -Wall -Wextra -Werror -std=c++98 -g

NAME		=	ircserv

RM			=	rm -f


SRCS_FILES	=	main.cpp \
				User.cpp \
				Server.cpp \
				Channel.cpp \
				server_utils.cpp \
				command.cpp \
				pass.cpp \
				nick.cpp \
				user.cpp \
				join.cpp \
				part.cpp \
				mode.cpp \
				topic.cpp \
				kick.cpp \
				invite.cpp \
				ping.cpp \
				quit.cpp \
				privmsg.cpp \
				info.cpp \
				motd.cpp \
				notice.cpp

OBJ_DIR		=	.obj
OBJS		=	${addprefix ${OBJ_DIR}/,${SRCS_FILES:.cpp=.o}}
DEPS		=	${addprefix ${OBJ_DIR}/,${SRCS_FILES:.cpp=.d}}


INCLUDES 	=	./includes
SRCS_PATH	=	./srcs


all:				${NAME}

${OBJ_DIR}/%.o:		${SRCS_PATH}/%.cpp
	@mkdir -p ${OBJ_DIR}
	@echo "Compiling $< ..."
	@${CXX} ${CXXFLAGS} -I ${INCLUDES} -c $< -o $@

${NAME}:	${OBJS}
	@echo "Compiling Object files..."
	@${CXX} ${CXXFLAGS} -o ${NAME} ${OBJS}

clean:
	@echo "Removing object files..."
	@cd ${OBJ_DIR}
	@${RM} ${OBJS} ${DEPS}

fclean:		clean
	@echo "Removing executable ${NAME}..."
	@${RM} ${NAME}

re:			fclean all

debug: CXXFLAGS += -DDEBUG=1
debug: all

-include ${DEPS}

.PHONY:		all clean fclean re
