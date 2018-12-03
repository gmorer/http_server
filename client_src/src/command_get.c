/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:24:25 by gmorer            #+#    #+#             */
/*   Updated: 2018/12/03 10:31:38 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int	fn_get(int sock, char **argv)
{
	t_envelope envelope;

	if (!argv[1])
	{
		write(2, "Bad usage, usage: put [FILE].\n", 30);
		return (0);
	}
	ft_memset(&envelope, 0, sizeof(t_envelope));
	envelope.status = 3;
	envelope.pending_size = 0;
	envelope.payload_size = ft_strlen(argv[1]) + 1;
	ft_memcpy(&(envelope.payload), argv[1], ft_strlen(argv[1]));
	send(sock, &envelope, get_envelope_size(ft_strlen(argv[1]) + 1), 0);
	recv(sock, &envelope, sizeof(t_envelope), 0);
	if (envelope.status == 40)
	{
		write(2, "No file.\n", 9);
		return (0);
	}
	receive_file(sock, argv[1], 1);
	return (1);
}
