#include "../inc/iosocket.h"

#define ERROR_MSG "getcwd() error"
#define ERROR_MSG_LEN 14

void *command_pwd(t_client *client)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        send_response(client->clientfd, cwd, ft_strlen(cwd));
    else if (send(client->clientfd, ERROR_MSG, ERROR_MSG_LEN, 0) == -1)
        printf("cant send\n");
    return "NULL";
}