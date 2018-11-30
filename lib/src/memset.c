/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memset.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:08:45 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:09:02 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftp.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*x;

	i = 0;
	x = b;
	while (i < len)
	{
		x[i] = (unsigned char)c;
		i++;
	}
	return (x);
}
