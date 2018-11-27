#include "iosocket.h"

void ft_strtsrfree(char **arg)
{
    size_t  index;

    index = 0;
    while (arg[index])
    {
        free(arg[index]);
        index += 1;
    }
    free(arg);
}