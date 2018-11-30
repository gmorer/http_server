/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strstrfree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:36:35 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:36:37 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "iosocket.h"

void	ft_strtsrfree(char **arg)
{
	size_t	index;

	index = 0;
	while (arg[index])
	{
		free(arg[index]);
		index += 1;
	}
	free(arg);
}
