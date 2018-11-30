/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:25:45 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:29:22 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

#define FT_P_CONNECT_FIRST_METHOD 1

#define FT_P_CONNECT_SECOND_METHOD 2

#define FT_P_CONNECT_METHOD FT_P_CONNECT_FIRST_METHOD

#if FT_P_CONNECT_METHOD == FT_P_CONNECT_FIRST_METHOD

int	connect_to_server(int sock, char *address, int port)
{
	struct addrinfo		*addr_info;
	struct sockaddr_in	client;

	(void)port;
	if (getaddrinfo(address, NULL, NULL, &addr_info) != 0)
		return (0);
	while (addr_info)
	{
		ft_memcpy(&client, addr_info->ai_addr, sizeof(addr_info));
		if (port)
			client.sin_port = htons(port);
		client.sin_family = AF_INET;
		printf("IP: %s\n", inet_ntoa(client.sin_addr));
		if (connect(sock, (struct sockaddr *)&client, sizeof(client)) == -1)
		{
			perror("connect");
			addr_info = addr_info->ai_next;
			continue ;
		}
		freeaddrinfo(addr_info);
		return (1);
	}
	freeaddrinfo(addr_info);
	return (0);
}

#endif

#if FT_P_CONNECT_METHOD == FT_P_CONNECT_SECOND_METHOD

int	connect_to_server(int sock, char *address, int port)
{
	struct hostent		*server;
	struct sockaddr_in	client;

	server = gethostbyname(address);
	if (!server)
	{
		printf("gethostbyname error\n");
		return (0);
	}
	client.sin_port = htons(port);
	client.sin_family = AF_INET;
	if (!server->h_addr_list[0])
		write(2, "Error\n", 6);
	ft_memcpy(&(client.sin_addr), server->h_addr_list[0], server->h_length);
	if (connect(sock, (struct sockaddr *)&client, sizeof(client)) == -1)
	{
		perror("connect");
		return (0);
	}
	return (1);
}

#endif
