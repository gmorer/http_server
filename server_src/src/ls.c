#include "../inc/iosocket.h"

void *command_ls(t_client *client)
{
    struct dirent *dp;
    DIR *rep;
    size_t response_size;

    rep = opendir(".");
    response_size = 0;
    while ((dp = readdir(rep)))
    {
        printf("size: %zu, name: %s\n", ft_strlen(dp->d_name), dp->d_name);
        response_size += ft_strlen(dp->d_name) + 1;
    }
    response_size -= 1; // last \n does not exist
    printf("response length: %zu\n", response_size);
    closedir(rep);
    (void)client;
    printf("LS\n");
    return "NULL";
}