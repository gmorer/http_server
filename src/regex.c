#include "server.h"

t_response error_404()
{
	return ((t_response){"Error 404", 9, 404});
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

t_response	 execute_response(t_client *client, t_endpoint *endpoints)
{
	int		index;
	t_endpoint	endpoint;
	regmatch_t	matches[10];

	index = 0;
	while ((*endpoints).url)
	{
		if (!regexec(&((*endpoints).comp_url), client->url, (*endpoints).args_no, matches, 0))
		{
			printf("match %s and %s\n", (*endpoints).url, client->url);
			printf("groups:\n");
			printf("length: %d\n", matches[1].rm_eo);
			while (index < 10/*(*endpoints).args_no*/)
			{
				write(1, client->url + matches[index].rm_so, matches[index].rm_eo);
				write(1, "\n", 1);
				index += 1;
			}
			printf("end of groups\n");
			return (*endpoints).action(client);
		}
		endpoints += 1;
	}
	return (error_404());
}
