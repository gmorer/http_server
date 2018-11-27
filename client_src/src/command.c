#include "ftp_client.h"

int fn_exit(int sock, char **argv)
{
    (void)sock;
    (void)argv;
    close(sock);
    exit(0);
}

int execut_command(int sock, char **argv)
{
    static t_command tab[COMMAND_TAB_LEN] = COMMAND_TAB;
    int index;

    index = 0;
    while (index < COMMAND_TAB_LEN)
    {
        if (ft_strcmp(argv[0], tab[index].name) == 0)
            return (tab[index].action(sock, argv));
        else
            index++;
    }
    if (argv[0])
    {
        write(1, "The command \"", 13);
        write(1, argv[0], ft_strlen(argv[0]));
        write(1, "\" does not exist.\n", 18);
    }
    return (0);
}