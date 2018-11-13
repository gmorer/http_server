#include "libftp.h"

char *ft_strncpy(char *dst, const char *src, size_t n)
{
    size_t j;

    j = 0;
    while (j < n && src[j] != '\0')
    {
        dst[j] = src[j];
        j++;
    }
    while (j < n)
    {
        dst[j] = '\0';
        j++;
    }
    return (dst);
}