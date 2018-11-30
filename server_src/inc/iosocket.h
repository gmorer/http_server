/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iosocket.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:34:51 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:34:55 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOSOCKET_H
# define IOSOCKET_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <dirent.h>
# include <limits.h>
# include <sys/stat.h>
# include "libftp.h"

# define BUFF_SIZE 4096
# define SOCKET_MAX_SIZE 1300
# define COMMAND_MAX 50
# define GOOD_RETURN ((void*)1)

typedef struct			s_client
{
	int					clientfd;
	struct sockaddr_in	client_addr;
	size_t				req_len;
	t_envelope			envelope;
}						t_client;

typedef void			*(*t_command_fun)(t_client *client, char **path);

extern int				g_socket_fd;
extern char				g_root_path[PATH_MAX];

void					*command_ls(t_client *client, char **path);
void					*command_cd(t_client *client, char **path);
void					*command_pwd(t_client *client, char **path);
void					*command_send(t_client *client, char **path);
void					*command_receive(t_client *client, char **path);
void					catch_sig(void);
void					*got_a_client(void *arg);
int						go_to(char **path, char *dest);
void					ft_strtsrfree(char **arg);
int						is_valid_path(char *path, int is_dir, int is_file);
char					*path_join(char *first, char *second);
int						remove_dots(char *path);

#endif
