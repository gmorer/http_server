/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:36:07 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:36:08 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/iosocket.h"

void	*command_receive(t_client *client, char **path)
{
	char		*file_name;
	size_t		total_size;
	size_t		path_len;

	if ((path_len = ft_strlen(*path)) && (*path)[path_len - 1] != '/')
	{
		total_size = path_len + 1 + client->envelope.payload_size;
		if (!(file_name = malloc(sizeof(char) * total_size + 1)))
			return (0);
		file_name[path_len] = '/';
		ft_memcpy(file_name + path_len + 1, client->envelope.payload,
				client->envelope.payload_size);
	}
	else
	{
		total_size = path_len + client->envelope.payload_size;
		if (!(file_name = malloc(sizeof(char) * total_size + 1)))
			return (0);
		ft_memcpy(file_name + path_len, client->envelope.payload,
				client->envelope.payload_size);
	}
	ft_memcpy(file_name, *path, path_len);
	file_name[total_size] = '\0';
	receive_file(client->clientfd, file_name, 0);
	return (GOOD_RETURN);
}
