#include "server.h"

char *get_code_string(int num)
{
	static t_method methods[] = METHODS_ARRAY;
	int             len;
	int             index;

	len = 58; // methode_array len;
	while (index < len && index > 0)
	{
		if (methods[index].number == num)
			return methods[index].name;
		index += 1;
	}
	return get_code_string(500);
}

char *get_header_value(t_client *client, char *field)
{
	int i;

	i = 0;
	while (i++ < client->header_len)
		if (strcmp(field, client->headers[i].field) == 0)
			return client->headers[i].value;
	return NULL;
}
