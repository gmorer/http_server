#ifndef COMMAND_ARRAY_H
#define COMMAND_ARRAY_H

#include "iosocket.h"

#define INIT_ARRAY                   \
    {                                \
        &invalid_command,     /*0*/  \
            &invalid_command, /*1*/  \
            &invalid_command, /*2*/  \
            &invalid_command, /*3*/  \
            &invalid_command, /*4*/  \
            &invalid_command, /*5*/  \
            &invalid_command, /*6*/  \
            &invalid_command, /*7*/  \
            &invalid_command, /*8*/  \
            &invalid_command, /*9*/  \
            &command_start,   /*10*/ \
            &command_close,   /*11*/ \
            &invalid_command, /*12*/ \
            &invalid_command, /*13*/ \
            &invalid_command, /*14*/ \
            &invalid_command, /*15*/ \
            &invalid_command, /*16*/ \
            &invalid_command, /*17*/ \
            &invalid_command, /*18*/ \
            &invalid_command, /*19*/ \
            &invalid_command, /*20*/ \
            &command_ls,      /*21*/ \
            &command_cd,      /*22*/ \
            &command_pwd,     /*23*/ \
            &invalid_command, /*24*/ \
            &invalid_command, /*25*/ \
            &invalid_command, /*26*/ \
            &invalid_command, /*27*/ \
            &invalid_command, /*28*/ \
            &invalid_command, /*29*/ \
            &invalid_command, /*30*/ \
            &command_send,    /*31*/ \
            &invalid_command, /*32*/ \
            &invalid_command, /*33*/ \
            &invalid_command, /*34*/ \
            &invalid_command, /*35*/ \
            &invalid_command, /*36*/ \
            &invalid_command, /*37*/ \
            &invalid_command, /*38*/ \
            &invalid_command, /*39*/ \
            &invalid_command, /*40*/ \
            &command_receive, /*41*/ \
            &invalid_command, /*42*/ \
            &invalid_command, /*43*/ \
            &invalid_command, /*44*/ \
            &invalid_command, /*45*/ \
            &invalid_command, /*46*/ \
            &invalid_command, /*47*/ \
            &invalid_command, /*48*/ \
            &invalid_command, /*49*/ \
    }

#endif