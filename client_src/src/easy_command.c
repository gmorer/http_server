#include "ftp_client.h"

int fn_ls(int sock, char **argv)
{
    (void)argv;
    return ask_server(sock, 21, NULL, 0);
}

int fn_cd(int sock, char **argv)
{
    if (!argv[1])
    {
        write(2, "Usage: cd [DIRECTORY]\n", 22);
        return (0);
    }
    return ask_server(sock, 22, argv[1], ft_strlen(argv[1]));
}

int fn_pwd(int sock, char **argv)
{
    (void)argv;
    return ask_server(sock, 23, NULL, 0);
    return (1);
}

int fn_help(int sock, char **argv)
{
    (void)sock;
    (void)argv;
    write(1, "Available command:\n\
    exit: quit the program\n\
    ls: list entity in the current directory\n\
    cd [DIRECTORY]: change the current directory on the server\n\
    pwd: print the path of the current directory\n\
    get: [FILENAME]: download the file\n\
    put: [FILENAME]: uplaud the file to the server\n\
    ", 19 + 23 + 41 + 59 + 45 + 34 + 47 + 25);
    return (1);
}