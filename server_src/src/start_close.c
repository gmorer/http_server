#include "../inc/iosocket.h"

void *command_start(t_client *client, char **path)
{
    (void)client;
    (void)path;
    printf("START\n");
    return "NULL";
}

void *command_close(t_client *client, char **path)
{
    (void)client;
    (void)path;
    printf("CLOSE\n");
    return "NULL";
}