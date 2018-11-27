#include "../inc/iosocket.h"

void *command_cd(t_client *client, char **path)
{
    t_envelope *envelope;

    envelope = (t_envelope *)client->buffer;
    
    if (go_to(path, envelope->payload))
    {
        send_success(client->clientfd, NULL, 0);
    }
    else
    {
        send_failure(client->clientfd, "Operation failure, getcwd() error.", 34);
    }
    return GOOD_RETURN;
}