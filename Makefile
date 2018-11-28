# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmorer <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/05/20 11:35:32 by gmorer            #+#    #+#              #
#    Updated: 2018/11/13 12:40:14 by gmorer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SERVER_NAME = server

CLIENT_NAME = client

LIB_NAME = libftp.a

LIB_DIR = lib

SERVER_DIR = server_src

CLIENT_DIR = client_src

.PHONY: all lib server client clean fclean re

all: lib server client

lib:
	make -C $(LIB_DIR)
	ln -sf ../$(LIB_DIR)/ $(SERVER_DIR)
	ln -sf ../$(LIB_DIR)/ $(CLIENT_DIR)

server: lib
	make -C $(SERVER_DIR)
	ln -sf $(SERVER_DIR)/$(SERVER_NAME) .

client: lib
	make -C $(CLIENT_DIR)
	ln -sf $(CLIENT_DIR)/$(CLIENT_NAME) .

clean:
	make -C $(LIB_DIR) clean
	make -C $(SERVER_DIR) clean
	make -C $(CLIENT_DIR) clean

fclean: clean
	rm -f $(SERVER_NAME) $(CLIENT_NAME) $(SERVER_DIR)/$(LIB_NAME)# $(CLIENT_DIR)/$(LIB_NAME)
	make -C $(LIB_DIR) fclean
	make -C $(SERVER_DIR) fclean
	make -C $(CLIENT_DIR) fclean

re: fclean all