#include "libftp.h"

char	*ft_strdup(const char *s1)
{
	char			*s2;
    size_t          length;

    length = ft_strlen(s1);
	if ((s2 = malloc(sizeof(char) * length + 1)) == NULL)
		return (NULL);
    ft_memcpy(s2, s1, length);
	return (s2);
}