#include "../inc/iosocket.h"

static char **enlarge_buffer(size_t *size, char **old_buffer)
{
    char **new_buffer;
    size_t new_size;

    new_size = *size + 30;
    if (!(new_buffer = malloc(sizeof(char *) * new_size)))
        return (NULL);
    ft_memcpy(new_buffer, old_buffer, sizeof(char *) * *size);
    free(old_buffer);
    *size += 30;
    return (new_buffer);
}

static void *send_result(int fd, char **files_buffer, size_t buffer_length, DIR *rep)
{
    char *payload;
    size_t total_size;
    size_t index;
    size_t offset;

    index = 0;
    total_size = buffer_length;
    while (index < buffer_length)
    {
        total_size += ft_strlen(files_buffer[index]);
        index++;
    }
    if (!(payload = malloc(sizeof(char) * (total_size + 1))))
        return (NULL); // send error
    index = 0;
    offset = 0;
    while (index < buffer_length)
    {
        ft_memcpy(payload + offset, files_buffer[index], ft_strlen(files_buffer[index]));
        offset += ft_strlen(files_buffer[index]) + 1;
        payload[offset - 1] = ' ';
        index++;
    }
    payload[total_size] = '\0';
    closedir(rep);
    send_response(fd, payload, total_size + 1);
    free(files_buffer);
    free(payload);
    return (NULL);
}

void *command_ls(t_client *client, char **path)
{
    struct dirent *dp;
    DIR *rep;
    size_t index;
    char **files_buffer;
    size_t buffer_length;

    index = 0;
    if (!(files_buffer = malloc(sizeof(char *) * 30)))
        return (NULL); // send error
    buffer_length = 30;
    if (!(rep = opendir(*path)))
        return (NULL); // send error
    while ((dp = readdir(rep)))
    {
        if (index + 1 >= buffer_length)
            if (!(files_buffer = enlarge_buffer(&buffer_length, files_buffer)))
                return (NULL); // send error
        files_buffer[index] = dp->d_name;
        index++;
    }
    send_result(client->clientfd, files_buffer, index, rep);
    return (GOOD_RETURN);
}