/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:10:02 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 15:38:19 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftp.h"

#define RST 1
#define SET 2
#define GET 3

size_t	get_envelope_size(size_t payload_size)
{
	static size_t	struct_size;

	struct_size = sizeof(t_envelope) - sizeof(char) * PAYLOAD_MAX_SIZE;
	return (struct_size + (sizeof(char) * payload_size));
}

int		ask_server(int sock, int status, char *payload, size_t payload_len)
{
	t_envelope	*request;
	char		buff[sizeof(t_envelope)];
	size_t		res_len;

	request = (t_envelope*)buff;
	fflush(stdout);
	if (payload_len > PAYLOAD_MAX_SIZE)
		return (0);
	ft_memset(request, 0, sizeof(t_envelope));
	request->status = status;
	request->payload_size = payload_len;
	if (!payload)
		payload_len = 0;
	if (payload_len)
		ft_memcpy(request->payload, payload, payload_len);
	if (send(sock, request, get_envelope_size(payload_len), 0) == -1)
		return (write(2, "Can't send this request.\n", 25) ? 0 : 0);
	res_len = recv(sock, buff, sizeof(t_envelope), 0);
	write(1, request->payload, request->payload_size);
	if (request->payload[request->payload_size - 1] != '\n')
		write(1, "\n", 1);
	return (1);
}

int		send_multiple_response(int fd, t_envelope envelope,
		size_t actual_size, size_t pending_size)
{
	size_t	response_length;

	envelope.payload_size = actual_size;
	envelope.pending_size = pending_size;
	envelope.status = 21;
	response_length = get_envelope_size(actual_size);
	if (send(fd, &envelope, response_length, 0) == -1)
		return (0);
	if (recv(fd, &envelope, sizeof(t_envelope), 0) == -1)
		return (0);
	return (1);
}

int		send_response(int fd, char *buffer, size_t buffer_length)
{
	t_envelope	envelope;
	size_t		index;
	size_t		copied;
	size_t		response_length;

	index = 0;
	while (index < buffer_length)
	{
		copied = buffer_length - index < PAYLOAD_MAX_SIZE
			? buffer_length - index : PAYLOAD_MAX_SIZE;
		ft_memcpy(envelope.payload, buffer + index, copied);
		envelope.payload_size = copied;
		envelope.pending_size = buffer_length - (index + copied);
		envelope.status = 20;
		response_length = get_envelope_size(copied);
		if (send(fd, &envelope, response_length, 0) == -1)
			return (0);
		index += copied;
	}
	return (1);
}
