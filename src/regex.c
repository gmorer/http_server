#include "server.h"

t_response response_error(char *error_msg, int http_code)
{
	return ((t_response){strdup(error_msg), strlen(error_msg), http_code});
}

t_endpoint	*keep_endpoints(t_endpoint *endpoints)
{
	static t_endpoint *saved = NULL;

	if (endpoints)
		saved = endpoints;
	return saved;
}

int compil_regex(t_endpoint *endpoints)
{
	while ((*endpoints).url)
	{
		printf("endpoint: %s\n", (*endpoints).url);
		if (regcomp(&((*endpoints).comp_url), (*endpoints).url, REG_EXTENDED))
		{
			dprintf(2, "error while compilation of the url: %s\n", (*endpoints).url);
			return (0);
		}
		endpoints += 1;
	}
	return (1);
}

static t_response	malloc_error()
{
	ALLOCATION_ERROR;
	return (response_error("Allocation error.", 500));
}

t_response	execute_response(t_client *client, t_endpoint *endpoints)
{
	int		index;
	t_endpoint	endpoint;
	regmatch_t	matches[10];

	index = 0;
	while ((*endpoints).url)
	{
		if (!regexec(&((*endpoints).comp_url), client->url, (*endpoints).args_no + 1, matches, 0))
		{
			client->params_length = (*endpoints).args_no;
			if (!client->params_length)
				return (*endpoints).action(client);
			client->params = malloc(sizeof(regmatch_t) * client->params_length);
			if (!client->params)
				return malloc_error();
			memcpy(client->params, matches + 1, client->params_length * sizeof(regmatch_t));
			return (*endpoints).action(client);
		}
		endpoints += 1;
	}
	return (response_error("Error 404.", 404));
}
