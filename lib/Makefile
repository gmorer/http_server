# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmorer <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/01 11:36:51 by gmorer            #+#    #+#              #
#    Updated: 2017/01/11 11:27:27 by gmorer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftp.a
CC = clang
CFLAGS= -Weverything
CPATH = src/
HPATH = inc/
OPATH = obj/
CFILES = send.c\
			memcpy.c\
			strlen.c
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
	$(CC) $(INC) $< -c -o $@

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all