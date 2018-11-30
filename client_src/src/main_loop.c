/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:33:16 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:33:33 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int	main_loop(int sock)
{
	char *buff;
	char **argv;

	while (1)
	{
		write(1, "ft_p > ", 7);
		get_next_line(0, &buff);
		if (!(argv = split_args(buff)))
		{
			write(1, "Malloc error\n", 13);
			return (0);
		}
		execut_command(sock, argv);
		free(buff);
	}
	return (1);
}
