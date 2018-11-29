#include "../inc/iosocket.h"

void *command_send(t_client *client, char **path)
{
    char *file_path;

    file_path = path_join(*path, client->envelope.payload);
    remove_dots(file_path);
    if (!is_valid_path(file_path, 0, 1))
        return NULL;
    send_file(client->clientfd, file_path, 0);
    free(file_path);
    return GOOD_RETURN;
}