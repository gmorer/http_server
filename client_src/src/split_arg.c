#include "ftp_client.h"

static int  is_space(char c)
{
    if (c == '\t' || c == ' ')
		return (1);
	else
		return (0);
}

static int	ft_words(char const *s)
{
	int		words;

	words = 0;
	while (*s)
	{
		if (!is_space(*s) && *s)
		{
			++words;
			while (!is_space(*s) && *s)
				++s;
		}
		else
			++s;
	}
	return (words);
}

static void	ft_split(char const *s, char ***result)
{
	int j;
	int i;

	i = 0;
	while (*s)
	{
		while (is_space(*s) && *s)
			++s;
		j = 0;
		while (!is_space(*(s + j)) && *(s + j))
			++j;
		if (j)
		{
			(*result)[i] = malloc(sizeof(char) * (j + 1));
			if (!(*result)[i])
				return ;
			j = 0;
			while (!is_space(*s) && *s)
				(*result)[i][j++] = *s++;
			(*result)[i++][j] = '\0';
		}
	}
	(*result)[i] = NULL;
}

char		**split_args(char const *s)
{
	char	**result;

	if (!s)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (ft_words(s) + 1));
	if (!result)
		return (NULL);
	ft_split(s, &result);
	return (result);
}