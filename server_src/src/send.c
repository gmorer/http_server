/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:36:16 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:36:17 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/iosocket.h"

void	*command_send(t_client *client, char **path)
{
	char	*file_path;

	file_path = path_join(*path, client->envelope.payload);
	remove_dots(file_path);
	if (!is_valid_path(file_path, 0, 1))
		return (NULL);
	send_file(client->clientfd, file_path, 0);
	free(file_path);
	return (GOOD_RETURN);
}
