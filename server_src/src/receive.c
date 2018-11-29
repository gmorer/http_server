#include "../inc/iosocket.h"

void *command_receive(t_client *client, char **path)
{
    char       *file_name;
    size_t      total_size;
    size_t      path_len;

    path_len = ft_strlen(*path);
    if ((*path)[path_len - 1] != '/')
    {
        total_size = path_len + 1 + client->envelope.payload_size;
        if (!(file_name = malloc(sizeof(char) * total_size + 1)))
            return (0); // malloc error
        ft_memcpy(file_name, *path, path_len);
        file_name[path_len] = '/';
        ft_memcpy(file_name + path_len + 1, client->envelope.payload, client->envelope.payload_size);
    }
    else
    {
        total_size = path_len + client->envelope.payload_size;
        if (!(file_name = malloc(sizeof(char) * total_size + 1)))
            return (0); // malloc error
        ft_memcpy(file_name, *path, path_len);
        ft_memcpy(file_name + path_len, client->envelope.payload, client->envelope.payload_size);
    }
    file_name[total_size] = '\0';
    receive_file(client->clientfd, file_name, 0);
    return GOOD_RETURN;
}