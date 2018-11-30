/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:35:34 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:35:35 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/iosocket.h"

#define PORT 8080

char	g_root_path[PATH_MAX];
int		g_socket_fd;

static int	perror_return(char *message)
{
	write(2, message, ft_strlen(message));
	return (1);
}

static int	main_loop(void)
{
	unsigned int		addrlen;
	t_client			*client;
	pthread_t			tid;

	addrlen = sizeof(struct sockaddr_in);
	while (1)
	{
		client = malloc(sizeof(t_client));
		ft_memset(client, 0, sizeof(client));
		printf("Listening...\n");
		client->clientfd = accept(g_socket_fd,
				(struct sockaddr *)&(client->client_addr), &addrlen);
		pthread_create(&tid, NULL, got_a_client, (void *)client);
	}
	close(g_socket_fd);
	return (1);
}

int			main(void)
{
	struct sockaddr_in	server;
	int					truee;

	if ((g_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (1);
	catch_sig();
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;
	getcwd(g_root_path, PATH_MAX);
	if (bind(g_socket_fd, (struct sockaddr *)&server, sizeof(server)) != 0)
		return (perror_return("Error with socket--bind.\n"));
	truee = 1;
	if (setsockopt(g_socket_fd, SOL_SOCKET, SO_REUSEADDR, &truee,
				sizeof(g_socket_fd)) == -1)
		return (perror_return("Error with Setsockopt.\n"));
	if (listen(g_socket_fd, 20) != 0)
		return (perror_return("Error with socket--listen.\n"));
	return (main_loop());
}
