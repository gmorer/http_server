#include "libftp.h"

int		ft_strcmp(const char *s1, const char *s2)
{
	int i;

	if (!s1)
	{
		if (!s2)
			return (0);
		else
			return (1);
	}
	else if (!s2)
		return (-1);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0')
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}