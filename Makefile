# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmorer <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/05/20 11:35:32 by gmorer            #+#    #+#              #
#    Updated: 2018/11/08 16:55:51 by gmorer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

CC = gcc

CFLAGS = -Werror -Wall -Wextra -g

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
	start_close.c

OPATH = obj/

OFILES = $(CFILES:.c=.o)

OBJ = $(addprefix $(OPATH), $(OFILES))

HPATH = inc/

HFILES = \
	inc/iosocket.h

INC = $(addprefix -I , $(HPATH))

.PHONY: all install clean fclean re

all: $(NAME)

$(NAME): $(OPATH) $(OBJ)
	$(CC) $(CFLAGS) $(LIBFLAG) $(OBJ) -o $(NAME)

$(OPATH):
	mkdir -p $(OPATH)

$(OPATH)%.o: $(CPATH)%.c $(HFILES)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
