#include "../inc/iosocket.h"

void *command_cd(t_client *client)
{
    t_envelope *envelope;

    envelope = (t_envelope *)client->buffer;
    if (chdir(envelope->payload) == -1)
    {
        send_failure(client->clientfd, "Operation failure, getcwd() error.", 34);
    }
    else
    {
        send_success(client->clientfd, NULL, 0);
    }
    return NULL;
}