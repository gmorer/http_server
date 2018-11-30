/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:35:01 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:35:04 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/iosocket.h"

void	*command_cd(t_client *client, char **path)
{
	client->envelope.payload[client->envelope.payload_size] = '\0';
	if (go_to(path, client->envelope.payload))
	{
		send_success(client->clientfd, NULL, 0);
	}
	else
	{
		send_failure(client->clientfd,
				"Operation failure, getcwd() error.", 34);
	}
	return (GOOD_RETURN);
}
