/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:36:16 by gmorer            #+#    #+#             */
/*   Updated: 2018/12/03 14:23:05 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/iosocket.h"

void	*command_send(t_client *client, char **path)
{
	char		*file_path;
	t_envelope	envelope;

	file_path = path_join(*path, client->envelope.payload);
	remove_dots(file_path);
	if (!is_valid_path(file_path, 0, 1))
	{
		ft_memset(&envelope, 0, sizeof(t_envelope));
		envelope.status = 40;
		send(client->clientfd, &envelope, get_envelope_size(0), 0);
		return (GOOD_RETURN);
	}
	envelope.status = 20;
	send(client->clientfd, &envelope, get_envelope_size(0), 0);
	send_file(client->clientfd, file_path, 0);
	free(file_path);
	return (GOOD_RETURN);
}
