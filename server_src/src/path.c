/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:35:44 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:35:47 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "iosocket.h"

char	g_root_path[PATH_MAX];

char		*path_join(char *first, char *second)
{
	char	add_size;
	size_t	st_len;
	size_t	sd_len;
	char	*rslt;

	if (ft_strcmp(second, "/") == 0)
		return (ft_strdup("/"));
	add_size = 1;
	st_len = ft_strlen(first);
	sd_len = ft_strlen(second);
	add_size -= first[st_len - 1] == '/';
	add_size -= second[0] == '/';
	add_size -= second[sd_len - 1] == '/';
	if ((rslt = malloc((st_len + sd_len + add_size + 1)
					* sizeof(char))) == NULL)
		return (NULL);
	rslt[st_len + sd_len + add_size] = '\0';
	ft_memset(rslt, 0, (st_len + sd_len + add_size) * sizeof(char));
	ft_memcpy(rslt, first, first[st_len - 1] == '/' ? --st_len : st_len);
	rslt[st_len] = '/';
	ft_memcpy(rslt + st_len + 1, second + (second[0] == '/'), sd_len
			+ (second[0] == '/' ? -1 : 0)
			+ (second[sd_len - 1] == '/' ? -1 : 0));
	return (rslt);
}

static int	remove_dots_sd(char *path, size_t index, char **split)
{
	size_t	second_index;
	size_t	head;

	second_index = 0;
	head = 0;
	while (second_index < index)
	{
		if (split[second_index])
		{
			path[head++] = '/';
			ft_memcpy(path + head, split[second_index],
					ft_strlen(split[second_index]));
			head += ft_strlen(split[second_index]);
		}
		second_index += 1;
	}
	path[head] = '\0';
	ft_strtsrfree(split);
	return (1);
}

int			remove_dots(char *path)
{
	char	**split;
	size_t	index;
	size_t	second_index;

	if ((index = 0) || !ft_strstr(path, ".."))
		return (1);
	split = ft_strsplit(path, '/');
	while (split[index])
	{
		if (ft_strcmp(split[index], "..") == 0)
		{
			free(split[index]);
			split[index] = NULL;
			second_index = index;
			while (second_index && !split[second_index])
				second_index -= 1;
			if (split[second_index])
			{
				free(split[second_index]);
				split[second_index] = NULL;
			}
		}
		index++;
	}
	return (remove_dots_sd(path, index, split));
}

int			is_valid_path(char *path, int is_directory, int is_file)
{
	struct stat	file_stat;

	if (ft_strncmp(g_root_path, path, ft_strlen(g_root_path)) != 0)
		return (0);
	if (is_directory || is_file)
	{
		if (stat(path, &file_stat) < 0)
			return (0);
	}
	if (is_directory)
	{
		if ((S_ISDIR(file_stat.st_mode) == 0))
			return (0);
		if (!(file_stat.st_mode & S_IXUSR))
			return (0);
	}
	else if (is_file)
	{
		if (!(file_stat.st_mode & S_IRUSR))
			return (0);
	}
	return (1);
}

int			go_to(char **path, char *dest)
{
	char	*new_path;

	if (!path || !*path || !dest)
	{
		return (0);
	}
	if (ft_strcmp(dest, ".") == 0)
		return (1);
	new_path = path_join(*path, dest);
	remove_dots(new_path);
	if (is_valid_path(new_path, 1, 0))
	{
		free(*path);
		*path = new_path;
		return (1);
	}
	return (0);
}
