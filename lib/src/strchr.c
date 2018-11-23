char	*ft_strchr(const char *s, int c)
{
	int i;

	i = 0;
	while (s[i] != c)
	{
		if (s[i] == '\0')
			return ((void*)0);
		i++;
	}
	return (char*)(&s[i]);
}