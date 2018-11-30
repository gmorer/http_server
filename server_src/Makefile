# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmorer <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/05/20 11:35:32 by gmorer            #+#    #+#              #
#    Updated: 2018/11/30 12:34:30 by gmorer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

CC = gcc

CFLAGS = -Werror -Wall -Wextra -g -fsanitize=address

LIBFLAG = -lpthread

CPATH = src/

CFILES = \
	main.c \
	signal.c \
	client.c \
	cd.c	\
	ls.c\
	pwd.c\
	receive.c\
	send.c\
	path.c\
	strstrfree.c

OPATH = obj/

OFILES = $(CFILES:.c=.o)

LIB = lib/libftp.a

OBJ = $(addprefix $(OPATH), $(OFILES))

HPATH = inc/\
		lib/inc

HFILES = \
	inc/iosocket.h

INC = $(addprefix -I , $(HPATH))

.PHONY: all install clean fclean re

all: $(NAME)

$(NAME): $(OPATH) $(OBJ)
	$(CC) $(CFLAGS) $(LIBFLAG) $(OBJ) -o $(NAME) $(LIB)

$(OPATH):
	mkdir -p $(OPATH)

$(OPATH)%.o: $(CPATH)%.c $(HFILES)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
