/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:36:22 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:36:27 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "../inc/iosocket.h"

int	g_socket_fd;

void	quit(int sig)
{
	(void)sig;
	close(g_socket_fd);
	exit(1);
}

void	catch_sig(void)
{
	signal(SIGINT, &quit);
	signal(SIGABRT, &quit);
	signal(SIGBUS, &quit);
	signal(SIGILL, &quit);
	signal(SIGINT, &quit);
	signal(SIGKILL, &quit);
	signal(SIGTERM, &quit);
	signal(SIGTRAP, &quit);
	return ;
}
