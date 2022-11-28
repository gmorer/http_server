# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmorer <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/05/20 11:35:32 by gmorer            #+#    #+#              #
#    Updated: 2019/03/02 22:11:34 by tet              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libserver.a

CC = gcc

CFLAGS = -g3 -fsanitize=address,undefined -Werror -Wall -Wextra -Wtrivial-auto-var-init

LIBFLAG = -lpthread -lc -luring

CPATH = src/

CFILES = \
	client.c \
	utils.c \
	http_errors.c \
	server.c \
	regex.c \
	worker.c \
	io.c

OPATH = obj/

OFILES = $(CFILES:.c=.o)

OBJ = $(addprefix $(OPATH), $(OFILES))
# OBJ += obj/http_parser.o

HPATH = inc/ \
		http_parser/

HFILES = \
	inc/server.h \
	inc/http_method.h \
	inc/utils.h \
	inc/http_errors.h \
	inc/worker.h \
	http_parser/http_parser.h

INC = $(addprefix -I./, $(HPATH))

.PHONY: all install clean fclean re test

all: http-parser $(NAME)

$(NAME): $(OPATH) $(OBJ) http-parser
	ar -rc $(NAME) $(OBJ) obj/http_parser.o
	ranlib $(NAME)

$(OPATH):
	mkdir -p $(OPATH)

$(OPATH)%.o: $(CPATH)%.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $@ $<

http-parser: obj/http_parser.o
obj/http_parser.o: $(OPATH) http_parser/http_parser.c
	cd http_parser
	cc  -I./http_parser/ -DHTTP_PARSER_STRICT=1 -Wall -Wextra -Werror -O0 -g -c http_parser/http_parser.c -o obj/http_parser.o

clean:
	rm -f $(OBJ)
	rm -rf obj/http_parser.o

fclean: clean
	rm -f $(NAME)

re: fclean all

test: ${NAME} test/main.c
	$(CC) test/main.c -g3 -fsanitize=address,undefined -Iinc/ ${NAME} -lpthread inc/http_server.h -o server

