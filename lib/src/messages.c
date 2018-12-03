/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:09:10 by gmorer            #+#    #+#             */
/*   Updated: 2018/12/03 14:23:40 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftp.h"

void	socket_error_msg(void)
{
	write(2, "Socket error.\n", 14);
}

int		file_error(int is_client)
{
	if (is_client)
		write(1, "No file.\n", 9);
	return (0);
}

int		send_error(int sockfd)
{
	t_envelope	envelope;

	envelope.status = 40;
	send(sockfd, &envelope, get_envelope_size(0), 0);
	return (0);
}

int		send_success(int fd, char *message, size_t length)
{
	t_envelope envelope;

	if (!message)
	{
		message = "Operation success.";
		length = 18;
	}
	envelope.status = 20;
	ft_strncpy(envelope.payload, message, PAYLOAD_MAX_SIZE);
	envelope.pending_size = 0;
	envelope.payload_size = length;
	if (send(fd, &envelope, get_envelope_size(length), 0) == -1)
	{
		socket_error_msg();
		return (0);
	}
	return (1);
}

int		send_failure(int fd, char *message, size_t length)
{
	t_envelope envelope;

	if (!message)
	{
		message = "Operation failure.";
		length = 18;
	}
	if (!length)
		length = ft_strlen(message);
	envelope.status = 44;
	ft_strncpy(envelope.payload, message, PAYLOAD_MAX_SIZE);
	envelope.pending_size = 0;
	envelope.payload_size = length;
	if (send(fd, &envelope, get_envelope_size(length), 0) == -1)
	{
		socket_error_msg();
		return (0);
	}
	return (0);
}
