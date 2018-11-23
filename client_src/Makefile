# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmorer <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/05/20 11:35:32 by gmorer            #+#    #+#              #
#    Updated: 2018/11/23 16:42:19 by gmorer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = client

CC = gcc

CFLAGS = -Werror -Wall -Wextra -g -fsanitize=address

CPATH = src/

CFILES = \
	main.c\
	connect.c\
	main_loop.c\
	command.c\
	easy_command.c\
	command_get.c\
	command_put.c\
	split_arg.c

OPATH = obj/

OFILES = $(CFILES:.c=.o)

LIB = lib/libftp.a

OBJ = $(addprefix $(OPATH), $(OFILES))

HPATH = inc/\
		lib/inc

HFILES = \
	inc/ftp_client.h

INC = $(addprefix -I , $(HPATH))

.PHONY: all install clean fclean re

all: $(NAME)

$(NAME): $(OPATH) $(OBJ)
	$(CC) $(CFLAGS) $(LIB) $(OBJ) -o $(NAME)

$(OPATH):
	mkdir -p $(OPATH)

$(OPATH)%.o: $(CPATH)%.c $(HFILES)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
