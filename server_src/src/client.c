/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:35:10 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 15:47:12 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/iosocket.h"

#define INVALID_RESPONSE "Invalid protocol, protocol on this server: FT_FTP1"
#define INVALID_RESPONSE_LEN 49

char	g_root_path[PATH_MAX];

static void	*invalid_command(t_client *client, char **path)
{
	(void)path;
	send_failure(client->clientfd, INVALID_RESPONSE, INVALID_RESPONSE_LEN);
	return (NULL);
}

static void	*command_dispatcher(int ftp_code, t_client *client, char **path)
{
	static const t_command_fun	command[COMMAND_MAX] = {
	&command_ls,
	&command_cd,
	&command_pwd,
	&command_send,
	&command_receive
	};

	return (command[ftp_code](client, path));
}

static void	*response(t_client *client, char **path)
{
	if (client->req_len < 5)
		return (invalid_command(client, NULL));
	if (client->envelope.status < 0 || client->envelope.status > COMMAND_MAX)
		return (invalid_command(client, NULL));
	return (command_dispatcher(client->envelope.status, client, path));
}

void		*got_a_client(void *arg)
{
	t_client	*client;
	char		*path;

	path = ft_strdup(g_root_path);
	client = (t_client *)arg;
	printf("%s:%d connected\n", inet_ntoa(client->client_addr.sin_addr),
			ntohs(client->client_addr.sin_port));
	while ((client->req_len = recv(client->clientfd, (void*)&(client->envelope),
					sizeof(t_envelope), 0)))
	{
		if (!response(client, &path))
			break ;
		ft_memset((void*)&(client->envelope), 0, sizeof(t_envelope));
	}
	close(client->clientfd);
	free(path);
	write(1, "Client disconnected.\n", 21);
	free(client);
	return (NULL);
}
