#include "ftp_client.h"

int fn_get(int sock, char **argv)
{
    (void)sock;
    (void)argv;

    write(1, "get\n", 4);
    return (1);
}