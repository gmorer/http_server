/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   easy_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:29:33 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 15:39:50 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int	fn_ls(int sock, char **argv)
{
	(void)argv;
	return (ask_server(sock, 0, NULL, 0));
}

int	fn_cd(int sock, char **argv)
{
	if (!argv[1])
	{
		write(2, "Usage: cd [DIRECTORY]\n", 22);
		return (0);
	}
	return (ask_server(sock, 1, argv[1], ft_strlen(argv[1])));
}

int	fn_pwd(int sock, char **argv)
{
	(void)argv;
	return (ask_server(sock, 2, NULL, 0));
}

int	fn_help(int sock, char **argv)
{
	(void)sock;
	(void)argv;
	printf("Available command:\n\
			exit: quit the program\n\
			ls: list entity in the current directory of the server\n\
			cd [DIRECTORY]: change the current directory on the server\n\
			pwd: print the path of the current directory of the server\n\
			get: [FILENAME]: download the file\n\
			put: [FILENAME]: uplaud the file to the server\n\
			lls: list entity in the current local directory\n\
			lcd [DIRECTORY]: change the current local directory\n\
			lpwd: print the path of the current local directory\n");
	return (1);
}
