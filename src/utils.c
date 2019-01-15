#include "server.h"

char	*get_header_value(t_client *client, char *field)
{
	int i;

	i = 0;
	while (i++ < client->private.header_line)
		if (strcmp(field, client->private.headers[i].field) == 0)
			return client->private.headers[i].value;
	return NULL;
}