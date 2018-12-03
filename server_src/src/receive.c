/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:36:07 by gmorer            #+#    #+#             */
/*   Updated: 2018/12/03 10:05:09 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/iosocket.h"

void	*command_receive(t_client *client, char **path)
{
	char		*file_name;

	file_name = path_join(*path, client->envelope.payload);
	remove_dots(file_name);
	receive_file(client->clientfd, file_name, 0);
	free(file_name);
	return (GOOD_RETURN);
}
