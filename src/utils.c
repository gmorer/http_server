#include "../inc/server.h"

void panic(char *msg) {
	dprintf(2, "%s\n", msg);
	exit(EXIT_FAILURE);
}

char *get_code_string(int num)
{
	static t_method methods[] = METHODS_ARRAY;
	int             len = SIZE_OF(methods);
	int             index = 0;

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
	int i = 0;

	while (i++ < client->headers_len)
		if (strcmp(field, client->headers[i].field) == 0)
			return client->headers[i].value;
	return NULL;
}
