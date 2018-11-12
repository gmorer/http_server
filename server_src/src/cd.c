#include "../inc/iosocket.h"

void *command_cd(t_client *client)
{
    (void)client;
    printf("CD\n");
    return "NULL";
}