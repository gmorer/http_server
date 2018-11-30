/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:35:23 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:35:24 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/iosocket.h"

static char	**enlarge_buffer(size_t *size, char **old_buffer)
{
	char	**new_buffer;
	size_t	new_size;

	new_size = *size + 30;
	if (!(new_buffer = malloc(sizeof(char *) * new_size)))
		return (NULL);
	ft_memcpy(new_buffer, old_buffer, sizeof(char *) * *size);
	free(old_buffer);
	*size += 30;
	return (new_buffer);
}

static void	*send_result(int fd, char **files_buf,
		size_t buffer_length, DIR *rep)
{
	char	*payload;
	size_t	tt_size;
	size_t	i;
	size_t	offset;

	i = 0;
	tt_size = buffer_length;
	while (i < buffer_length)
		tt_size += ft_strlen(files_buf[i++]);
	if (!(payload = malloc(sizeof(char) * (tt_size + 1))))
		return (NULL);
	i = 0;
	offset = 0;
	while (i < buffer_length)
	{
		ft_memcpy(payload + offset, files_buf[i], ft_strlen(files_buf[i]));
		offset += ft_strlen(files_buf[i++]) + 1;
		payload[offset - 1] = ' ';
	}
	payload[tt_size] = '\0';
	closedir(rep);
	send_response(fd, payload, tt_size);
	free(files_buf);
	free(payload);
	return (NULL);
}

void		*command_ls(t_client *client, char **path)
{
	struct dirent	*dp;
	DIR				*rep;
	size_t			index;
	char			**files_buffer;
	size_t			buffer_length;

	index = 0;
	if (!(files_buffer = malloc(sizeof(char *) * 30)))
		return (NULL);
	buffer_length = 30;
	if (!(rep = opendir(*path)))
		return (NULL);
	while ((dp = readdir(rep)))
	{
		if (index + 1 >= buffer_length)
			if (!(files_buffer = enlarge_buffer(&buffer_length, files_buffer)))
				return (NULL);
		files_buffer[index] = dp->d_name;
		index++;
	}
	send_result(client->clientfd, files_buffer, index, rep);
	return (GOOD_RETURN);
}
