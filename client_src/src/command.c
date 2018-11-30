/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:23:37 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 15:52:11 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int	fn_exit(int sock, char **argv)
{
	(void)sock;
	(void)argv;
	close(sock);
	exit(0);
}

int	execut_command(int sock, char **argv)
{
	int					index;
	static t_command	tab[COMMAND_TAB_LEN] = {
		{"exit", fn_exit},
		{"ls", fn_ls},
		{"cd", fn_cd},
		{"pwd", fn_pwd},
		{"get", fn_get},
		{"put", fn_put},
		{"help", fn_help},
		{"lls", fn_lls},
		{"lcd", fn_lcd},
		{"lpwd", fn_lpwd},
	};

	index = 0;
	while (index < COMMAND_TAB_LEN)
	{
		if (ft_strcmp(argv[0], tab[index].name) == 0)
			return (tab[index].action(sock, argv));
		index++;
	}
	if (argv[0])
		printf("The command \"%s\" does not exist.\n", argv[0]);
	return (0);
}
