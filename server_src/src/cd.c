#include "../inc/iosocket.h"

void *command_cd(t_client *client)
{
    t_envelope *envelope;

    envelope = (t_envelope *)client->buffer;
    printf("client: %s\n", envelope->payload);
    if (chdir(envelope->payload) == -1)
        return NULL;
    (void)client;
    printf("CD\n");
    return NULL;
}