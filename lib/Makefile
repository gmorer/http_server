# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmorer <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/01 11:36:51 by gmorer            #+#    #+#              #
#    Updated: 2018/11/23 15:18:56 by gmorer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftp.a
CC = clang
CFLAGS= -Werror -Wextra -Wall -g
CPATH = src/
HPATH = inc/
OPATH = obj/
CFILES = send.c\
			memcpy.c\
			strlen.c\
			messages.c\
			strncpy.c\
			memset.c\
			atoi.c\
			get_next_line.c\
			strcat.c\
			strchr.c\
			strsub.c\
			strcmp.c\
			envelope.c\
			strsplit.c\
			strstr.c\
			strdup.c
HFILES = inc/libftp.h
INC = $(addprefix -I , $(HPATH))
OBJ = $(addprefix $(OPATH), $(CFILES:.c=.o))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME) : $(OBJ)
	ar -rc $(NAME) $(OBJ)
	ranlib $(NAME)

$(OPATH)%.o : $(CPATH)%.c $(HFILES)
	@mkdir -p $(OPATH)
	$(CC) $(CFLAGS) $(INC) $< -c -o $@

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all
