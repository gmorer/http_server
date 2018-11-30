/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:35:52 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:35:57 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/iosocket.h"

#define ERROR_MSG "getcwd() error"
#define ERROR_MSG_LEN 14

void	*command_pwd(t_client *client, char **path)
{
	send_response(client->clientfd, *path, ft_strlen(*path));
	return (GOOD_RETURN);
}
