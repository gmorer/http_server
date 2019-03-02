# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmorer <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/05/20 11:35:32 by gmorer            #+#    #+#              #
#    Updated: 2019/03/02 22:09:52 by tet              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libserver.a

CC = gcc

CFLAGS = -g # -Werror -Wall -Wextra

LIBFLAG = -lpthread

CPATH = src/

CFILES = \
	signal.c \
	client.c \
	utils.c \
	http_errors.c \
	parser_cb.c \
	server.c \
	regex.c

OPATH = obj/

OFILES = $(CFILES:.c=.o)

OBJ = $(addprefix $(OPATH), $(OFILES))
OBJ += obj/http_parser.o

HPATH = inc/ \
		http_parser/

HFILES = \
	inc/server.h \
	inc/http_method.h \
	inc/utils.h \
	inc/http_errors.h \
	http_parser/http_parser.h

INC = $(addprefix -I./, $(HPATH))

.PHONY: all install clean fclean re

all: http-parser $(NAME)

$(NAME): $(OPATH) $(OBJ)
	#$(CC) $(LIBFLAG) $(CFLAGS) $(OBJ) obj/http_parser.o -o $(NAME)
	ar -rc $(NAME) $(OBJ) 
	ranlib $(NAME)

$(OPATH):
	mkdir -p $(OPATH)

$(OPATH)%.o: $(CPATH)%.c $(HFILES)
	$(CC) $(INC) $(CFLAGS) -c -o $@ $<

http-parser: $(OPATH)
	cd http_parser
	cc  -I./http_parser/ -DHTTP_PARSER_STRICT=1 -Wall -Wextra -Werror -O0 -g -c http_parser/http_parser.c -o obj/http_parser.o

clean:
	rm -f $(OBJ)
	rm -rf obj/http_parser.o

fclean: clean
	rm -f $(NAME)

re: fclean all

test:
	$(CC) test/main.c -Iinc/ libserver.a -lpthread inc/http_server.h -o server
