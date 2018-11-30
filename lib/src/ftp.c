/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:07:52 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:07:53 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftp.h"

int	print_percent(float percent, int is_client, int is_finish)
{
	if (is_client && !is_finish)
	{
		printf("\33[2K\r");
		printf("%.2f%%", percent);
		fflush(stdout);
	}
	else if (is_client && is_finish)
	{
		write(1, "\33[2K\r100%\n", 10);
		write(1, "finish!\n", 8);
	}
	return (1);
}

int	send_finish(int sock_fd, int cclose)
{
	t_envelope	envelope;

	if (!cclose)
	{
		envelope.status = 20;
		envelope.payload_size = 0;
		envelope.pending_size = 0;
		return (send(sock_fd, &envelope, get_envelope_size(0), 0));
	}
	else
	{
		close(sock_fd);
		write(1, "Error while sending\n", 20);
		return (0);
	}
}

int	send_file(int sock_fd, char *file_path, int is_client)
{
	struct stat	file_stat;
	size_t		file_size;
	int			file_fd;
	int			read_ret;
	t_envelope	envelope;

	if (stat(file_path, &file_stat) == -1)
		return (0);
	file_size = file_stat.st_size;
	if ((file_fd = open(file_path, O_RDONLY)) == -1)
		return (0);
	recv(sock_fd, &envelope, sizeof(t_envelope), 0);
	ft_memset(&envelope, 0, sizeof(t_envelope));
	while ((read_ret = read(file_fd, envelope.payload, PAYLOAD_MAX_SIZE)) > 0)
	{
		print_percent((float)(file_stat.st_size - file_size)
				* 100 / (float)file_stat.st_size, is_client, 0);
		file_size -= read_ret;
		if (!send_multiple_response(sock_fd, envelope, read_ret, file_size))
			send_finish(file_fd, 1);
	}
	print_percent(0, is_client, 1);
	return (send_finish(sock_fd, 0) == -1 ? 0 : 1);
}

int	receive_one_chunk(int sock_fd, t_envelope *envelope)
{
	t_envelope	res_envelope;

	if (recv(sock_fd, envelope, sizeof(t_envelope), 0) == -1)
	{
		write(1, "recv error\n", 10);
		envelope->status = 40;
		return (40);
	}
	ft_memset(&res_envelope, 0, sizeof(t_envelope));
	res_envelope.status = 20;
	res_envelope.payload_size = 0;
	res_envelope.pending_size = 0;
	if (envelope->status != 21)
		return (envelope->status);
	if (send(sock_fd, &res_envelope, get_envelope_size(0), 0) == -1)
	{
		envelope->status = 40;
		return (0);
	}
	return (envelope->status);
}

int	receive_file(int sock_fd, char *file_path, int is_client)
{
	int			file_fd;
	char		status;
	char		buff[sizeof(t_envelope)];
	t_envelope	*envelope;
	size_t		file_size;

	file_size = 0;
	ft_memset(buff, 0, sizeof(t_envelope));
	envelope = (t_envelope*)buff;
	envelope->status = 20;
	if ((file_fd = open(file_path, O_WRONLY | O_APPEND | O_CREAT, 0644)) == -1)
		return (0);
	if (send(sock_fd, envelope, get_envelope_size(0), 0) == -1)
		return (0);
	while ((status = receive_one_chunk(sock_fd, envelope)) == 21)
	{
		file_size = file_size ? file_size
			: envelope->pending_size + envelope->payload_size;
		print_percent((float)(file_size - envelope->pending_size)
				* 100 / (float)file_size, is_client, 0);
		write(file_fd, envelope->payload, envelope->payload_size);
	}
	close(file_fd);
	print_percent(0, is_client, 1);
	return (envelope->status != 20 ? 0 : 1);
}
