/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:31:16 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:32:59 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int	main(int argc, char **argv)
{
	int sock;
	int port;

	if (argc < 3)
		printf("Usage: client [hostname] [PORT]\n");
	if (argc < 3)
		return (1);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return (1);
	}
	port = ft_atoi(argv[2]);
	if (!port || port < 0)
		printf("Invalid port.\n");
	if (!port || port < 0)
		return (1);
	if (connect_to_server(sock, argv[1], port))
		main_loop(sock);
	else
		printf("Cant connect :(\n");
	return (0);
}
