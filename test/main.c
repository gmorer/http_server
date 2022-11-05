// #include "../inc/http_server.h"
#include "../inc/server.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

t_response	error_404(t_client *client)
{
	return((t_response){strdup("Error 404"), 9, 404});
}

t_response	hello_world(t_client *client)
{
	return ((t_response){strdup("Hello world."), 12, 200});
}

t_response	sum_ep(t_client *client)
{
	int number_1;
	int number_2;
	size_t result_len;
	char *result;
	printf("ptr of client => %p\n", client);
	printf("ptr of client.url => %p\n", client->url);

	printf("params 1 end: %d, params 2 end: %d", client->params[0].rm_eo, client->params[1].rm_eo);
	
	client->url[client->params[0].rm_eo] = 0;
	client->url[client->params[1].rm_eo] = 0;
	printf("client url was ok\n");
	number_1 = atoi(client->url + client->params[0].rm_so);
	number_2 = atoi(client->url + client->params[1].rm_so);
	result_len = snprintf(NULL, 0, "%d", number_1 + number_2);
	result = calloc(result_len + 1, sizeof(char));
	snprintf(result, result_len, "%d", number_1 + number_2);
	return ((t_response){result, result_len, 200});
}

int main(int argc, char **argv)
{
	t_endpoint endpoints[] = {
		(t_endpoint){"^/$", 0, &hello_world, 0},
		(t_endpoint){"^/sum/(.*)/(.*)$", 2, &sum_ep, 0},
		(t_endpoint){"/", 0, &error_404, 0},
		ENDPOINTS_END
	};

	printf("calling server init\n");
	fflush(stdout);
	if (!init_server(argc > 1 ? atoi(argv[1]) : 0, endpoints))
	{
		dprintf(2, "Cannot initialize the server\n");
		return (1);
	}
	printf("server inito OK\n");
	launch_server();
	return (0);
}
