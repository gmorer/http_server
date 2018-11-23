#include "ftp_client.h"

int fn_put(int sock, char **argv)
{
    (void)sock;
    (void)argv;

    write(1, "put\n", 4);
    return (1);
}