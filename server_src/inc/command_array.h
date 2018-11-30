/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_array.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <gmorer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 13:34:39 by gmorer            #+#    #+#             */
/*   Updated: 2018/11/30 13:34:44 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_ARRAY_H
#define COMMAND_ARRAY_H

#include "iosocket.h"

#define INIT_ARRAY                   \
    {                                \
        &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &command_ls,      \
            &command_cd,      \
            &command_pwd,     \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &command_send,    \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &command_receive, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
            &invalid_command, \
    }

#endif
