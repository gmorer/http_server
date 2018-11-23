#include "libftp.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*x;
	size_t	i;

	if (!(x = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = start;
	while (i < start + len)
	{
		x[i - start] = s[i];
		i++;
	}
	x[i - start] = '\0';
	return (x);
}