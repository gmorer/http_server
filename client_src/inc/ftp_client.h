/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:20:57 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 15:50:17 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_CLIENT_H
# define FTP_CLIENT_H

# include <stdio.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <limits.h>
# include <dirent.h>
# include "libftp.h"

typedef int	(*t_command_fun)(int sock, char **argv);

typedef struct	s_command
{
	char			*name;
	t_command_fun	action;

}				t_command;

# define COMMAND_TAB_LEN 10

char			**split_args(char const *s);
int				main_loop(int sock);
int				connect_to_server(int sock, char *address, int port);
int				execut_command(int sock, char **argv);
int				fn_ls(int sock, char **argv);
int				fn_cd(int sock, char **argv);
int				fn_pwd(int sock, char **argv);
int				fn_put(int sock, char **argv);
int				fn_get(int sock, char **argv);
int				fn_help(int sock, char **argv);
int				fn_lls(int sock, char **argv);
int				fn_lcd(int sock, char **argv);
int				fn_lpwd(int sock, char **argv);

#endif
