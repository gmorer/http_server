/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_put.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:24:55 by gmorer            #+#    #+#             */
/*   Updated: 2018/12/03 11:00:49 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int	file_ok(char *file)
{
	struct stat buf;

	if (stat(file, &buf) == -1)
	{
		write(2, "Can't read the file information.\n", 33);
		return (0);
	}
	if (!(buf.st_mode & S_IRUSR))
	{
		write(2, "Invalid file permission.\n", 25);
		return (0);
	}
	if (!(S_ISREG(buf.st_mode)))
	{
		write(2, "This is not a file\n", 19);
		return (0);
	}
	return (1);
}

int	fn_put(int sock, char **argv)
{
	t_envelope envelope;

	if (!argv[1])
	{
		write(2, "Bad usage, usage: put [FILE].\n", 30);
		return (0);
	}
	if (!file_ok(argv[1]))
		return (0);
	ft_memset(&envelope, 0, sizeof(t_envelope));
	envelope.status = 4;
	envelope.pending_size = 0;
	envelope.payload_size = ft_strlen(argv[1]) + 1;
	ft_memcpy(&(envelope.payload), argv[1], ft_strlen(argv[1]));
	send(sock, &envelope, get_envelope_size(ft_strlen(argv[1]) + 1), 0);
	send_file(sock, argv[1], 1);
	return (1);
}
