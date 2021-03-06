#include "server.h"

int g_socket_sd;

int init_server(int port, t_endpoint *endpoints)
{
	int true;
	struct sockaddr_in server = {
	    .sin_family = AF_INET,
	    .sin_port = htons(port ? port : DEFAULT_PORT),
	    .sin_addr.s_addr = INADDR_ANY,
	};

	if ((g_socket_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (0);
	catch_sig();
	/*---Assign a port number to the socket---*/
	if (bind(g_socket_sd, (struct sockaddr *) &server, sizeof(server)) != 0)
	{
		perror("socket--bind");
		return (0);
	}
	/*---Dunno whats this is ---*/
	true = 1;
	if (setsockopt(g_socket_sd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(g_socket_sd)) == -1)
	{
		perror("Setsockopt");
		return (0);
	}
	/*---Make it a "listening socket"---*/
	if (listen(g_socket_sd, 20) != 0)
	{
		perror("socket--listen");
		return (0);
	}
	if (!compil_regex(endpoints))
		return (0);
	keep_endpoints(endpoints);
	return (1);
}

void launch_server(void)
{
	t_client *client;
	pthread_t tid;
	socklen_t len;

	/*
	 * The loop is wating on the accept line
	 * just the allocation is done before the
	 * request arrive
	 * */
	while (1)
	{
		len = sizeof(client->client_addr);
		client = malloc(sizeof(t_client));
		if (!client)
		{
			ALLOCATION_ERROR;
			continue;
		}
		bzero(client, sizeof(t_client));
		/*---accept a connection (creating a data pipe)---*/
		printf("waiting for connection...\n");
		client->clientfd = accept(g_socket_sd, (struct sockaddr *) &(client->client_addr), &len);
		pthread_create(&tid, NULL, got_a_client, (void *) client);
	}
}
