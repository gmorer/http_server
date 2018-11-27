#include "../inc/iosocket.h"

#define ERROR_MSG "getcwd() error"
#define ERROR_MSG_LEN 14

void *command_pwd(t_client *client, char **path)
{
    send_response(client->clientfd, *path, ft_strlen(*path));
    return GOOD_RETURN;
}