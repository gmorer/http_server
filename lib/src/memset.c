#include "libftp.h"

void *ft_memset(void *b, int c, size_t len)
{
    size_t i;
    unsigned char *x;

    i = 0;
    x = b;
    while (i < len)
    {
        x[i] = (unsigned char)c;
        i++;
    }
    return (x);
}