#include "../inc/iosocket.h"

void *command_cd(t_client *client, char **path)
{
    printf("payload_size: %zu\n", client->envelope.payload_size);
    printf("payload: %s\n", client->envelope.payload);
    client->envelope.payload[client->envelope.payload_size] = '\0';
    if (go_to(path, client->envelope.payload))
    {
        send_success(client->clientfd, NULL, 0);
    }
    else
    {
        send_failure(client->clientfd, "Operation failure, getcwd() error.", 34);
    }
    return GOOD_RETURN;
}