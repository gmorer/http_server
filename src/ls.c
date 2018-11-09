#include "../inc/iosocket.h"

void *command_ls(t_client *client)
{
    struct dirent *lecture;
    DIR *rep;
    size_t buff_size;
    char *buff;
    size_t index;

    index = 0;
    buff_size = 50;
    if (!(buff = malloc(buff_size)))
        return;
    rep = opendir(".");
    while ((lecture = readdir(rep)))
    {
        printf("%s\n", lecture->d_name);
    }
    closedir(rep);
    (void)client;
    printf("LS\n");
    return "NULL";
}