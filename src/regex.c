#include "../inc/server.h"

t_endpoint *g_endpoints = NULL;

t_response response_error(char *error_msg, int http_code) {
	return ((t_response){
		.body = strdup(error_msg),
		.body_len = strlen(error_msg),
		.http_code = http_code,
		.headers = NULL,
		.headers_len = 0,
	});
}

bool		compil_regex(t_endpoint *endpoints) {
	t_endpoint *save = endpoints;

	if (g_endpoints != NULL) {
		return false;
	}
	while ((*endpoints).url)
	{
		printf("endpoint: %s\n", (*endpoints).url);
		if (regcomp(&((*endpoints).comp_url), (*endpoints).url, REG_EXTENDED))
		{
			dprintf(2, "error while compilation of the url: %s\n", (*endpoints).url);
			return false;
		}
		endpoints += 1;
	}
	g_endpoints = save;
	return true;
}

static t_response malloc_error()
{
	ALLOCATION_ERROR;
	return (response_error("Allocation error.", 500));
}

t_response execute_response(t_client *client)
{
	regmatch_t	matches[10];
	t_endpoint *endpoints = g_endpoints;
	printf("ptr of client => %p\n", client);
	printf("ptr of client.url => %p\n", client->url);

	while ((*endpoints).url)
	{
		printf("==> testing %s with %s\n", client->url, (*endpoints).url);
		if (!regexec(&((*endpoints).comp_url), client->url, (*endpoints).args_no + 1, matches, 0)) {
			printf("args_no: %ld\n", (*endpoints).args_no);
			client->params_length = (*endpoints).args_no;
			if (!client->params_length) {
				return (*endpoints).action(client);
			}
			client->params = calloc(client->params_length, sizeof(regmatch_t));
			if (client->params == NULL) {
				return malloc_error();
			}
			printf("gonna do the memcpy\n");
			memcpy(client->params, &(matches[1]), client->params_length * sizeof(regmatch_t));
			printf("memcpy done, url: %s\n", client->url);
			printf("ptr after memcpy of client => %p\n", client);
			printf("ptr after memcpy of client.url => %p\n", client->url);
			return (*endpoints).action(client);
		}
		endpoints += 1;
	}
	printf("==> testing done\n");
	return (response_error("Error 404.", 404));
}
