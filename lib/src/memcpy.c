/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memcpy.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:08:14 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:08:41 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftp.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*y;
	char	*z;

	i = 0;
	y = (char *)dst;
	z = (char *)src;
	if (n == 0)
		return (y);
	while (i <= n)
	{
		y[i] = z[i];
		i++;
	}
	return (y);
}
