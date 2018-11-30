/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:30:28 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:30:50 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int	fn_lls(int sock, char **argv)
{
	struct dirent	*dp;
	DIR				*rep;

	(void)sock;
	(void)argv;
	if (!(rep = opendir(".")))
		return (0);
	while ((dp = readdir(rep)))
		printf("%s ", dp->d_name);
	fflush(stdout);
	write(1, "\n", 1);
	return (1);
}

int	fn_lcd(int sock, char **argv)
{
	(void)sock;
	if (!argv[1])
	{
		write(2, "Usage: lcd [DIRECTORY]\n", 23);
		return (0);
	}
	if (chdir(argv[1]) == -1)
	{
		write(2, "Cant access this directory.\n", 28);
		return (0);
	}
	else
		return (1);
}

int	fn_lpwd(int sock, char **argv)
{
	char cwd[PATH_MAX];

	(void)argv;
	(void)sock;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (1);
	}
	return (0);
}
