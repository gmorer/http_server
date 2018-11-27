#include "../inc/iosocket.h"

void *command_send(t_client *client, char **path)
{
    (void)client;
    (void)path;
    printf("SEND\n");
    return GOOD_RETURN;
}