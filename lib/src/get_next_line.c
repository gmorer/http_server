/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:06:40 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:07:31 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftp.h"

char		*ft_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	while ((dst[i] = src[i]))
		i++;
	return (dst);
}

static char	*ft_strnew(size_t size)
{
	char	*n;

	if ((n = malloc(sizeof(*n) * (size + 1))) == NULL)
		return (NULL);
	ft_memset(n, '\0', size + 1);
	return (n);
}

char		*ft_strdup(const char *s1)
{
	char			*s2;

	if ((s2 = malloc(sizeof(char) * ft_strlen(s1) + 1)) == NULL)
		return (NULL);
	s2 = ft_strcpy(s2, s1);
	return (s2);
}

static char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*x;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	j = ft_strlen(s1) + ft_strlen(s2) + 1;
	if (!(x = (char*)malloc(sizeof(char) * j)))
		return (NULL);
	ft_strcpy(x, s1);
	ft_strcat(x, s2);
	return (x);
}

int			get_next_line(int fd, char **line)
{
	char		*buf;
	char		*t;
	static char	*save[257];
	int			r;

	r = 0;
	if ((t = NULL) || fd < 0 || !line || GNL_BUFF_SIZE < 1 || fd > 256)
		return (-1);
	!save[fd] ? (save[fd] = ft_strnew(0)) : 0;
	if (!(buf = malloc(GNL_BUFF_SIZE + 1)) || save[fd] == NULL)
		return (-1);
	while (!(t = ft_strchr(save[fd], '\n')) &&
			((r = (int)read(fd, buf, GNL_BUFF_SIZE)) > 0))
	{
		buf[r] = '\0';
		(t = ft_strjoin(save[fd], buf)) ? free(save[fd]) : free(save[fd]);
		save[fd] = t;
	}
	free(buf);
	if (r < 0 || (!t && save[fd][0] == '\0'))
		return ((r < 0) ? -1 : 0);
	*line = (t ? ft_strsub(save[fd], 0, t - save[fd]) : ft_strdup(save[fd]));
	save[fd] = (t ? ft_strcpy(save[fd], ++t) : ft_strdup("\0"));
	return (1);
}
