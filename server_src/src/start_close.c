#include "../inc/iosocket.h"

void *command_start(t_client *client)
{
    (void)client;
    printf("START\n");
    return "NULL";
}

void *command_close(t_client *client)
{
    (void)client;
    printf("CLOSE\n");
    return "NULL";
}