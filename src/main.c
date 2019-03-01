#include "server.h"


t_response	hello_world(t_client *client)
{
	size_t	index;

	index = 0;
	printf("params length = %d\n", client->params_length);
	while (index < client->params_length)
	{
		write(1, client->url + client->params[index].rm_so, client->params[index].rm_eo - client->params[index].rm_so);
		write(1, "\n", 1);
		index += 1;
	}
	printf("Hello world function\n");
	return ((t_response){NULL, 0, 0});
}

int main(int argc, char **argv)
{
	t_endpoint endpoints[4] = {
		(t_endpoint){"/(.*)/test", 1, &hello_world, 0},
		(t_endpoint){"^/$", 0, &hello_world, 0},
		(t_endpoint){"/hello_world", 0, &hello_world, 0},
		(t_endpoint){NULL, 0, 0, 0}
	};

	if (!init_server(argc > 1 ? atoi(argv[1]) : 0, endpoints))
	{
		write(2, "Cannot initialize the server\n", 29);
		return (1);
	}
	launch_server();
	return (0);
}
