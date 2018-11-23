#ifndef FTP_CLIENT_H
#define FTP_CLIENT_H

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "libftp.h"

typedef int (*t_command_fun)(int sock, char **argv);

typedef struct s_command
{
    char            *name;
    t_command_fun   action;

}               t_command;

#define COMMAND_TAB_LEN 7
#define COMMAND_TAB            \
        {                      \
            {"exit", fn_exit}, \
            {"ls", fn_ls},     \
            {"cd", fn_cd},     \
            {"pwd", fn_pwd},   \
            {"get", fn_get},   \
            {"put", fn_put},   \
            {"help", fn_help},   \
        }

char **split_args(char const *s);
int main_loop(int sock);
int connect_to_server(int sock, char *address, int port);
int execut_command(int sock, char **argv);
int fn_ls(int sock, char **argv);
int fn_cd(int sock, char **argv);
int fn_pwd(int sock, char **argv);
int fn_put(int sock, char **argv);
int fn_get(int sock, char **argv);
int fn_help(int sock, char **argv);

#endif
