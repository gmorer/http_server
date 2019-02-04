#include "server.h"

int g_socket_sd;

int	init_server(int port) {
	struct sockaddr_in				server;
	int								true;

	if ((g_socket_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		return (0);
	catch_sig();
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port ? port : DEFAULT_PORT);
	server.sin_addr.s_addr = INADDR_ANY;
	/*---Assign a port number to the socket---*/
    if (bind(g_socket_sd, (struct sockaddr*)&server, sizeof(server)) != 0 )
	{
		perror("socket--bind");
		return (0);
	}
	/*---Dunno whats this is ---*/
	true = 1;
	if (setsockopt(g_socket_sd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(g_socket_sd)) == -1)
	{
    	perror("Setsockopt");
		return(0);
	}
	/*---Make it a "listening socket"---*/
	if (listen(g_socket_sd, 20) != 0 )
	{
		perror("socket--listen");
		return (0);
	}
	return (1);
}

void launch_server(void) {
	t_client					*client;
	pthread_t					tid;
	socklen_t					len;

	/*---Forever... ---*/
	while (1)
	{
		len = sizeof(client->client_addr);
		client = malloc(sizeof(t_client));
		if (!client)
		{
			ALLOCATION_ERROR;
			continue ;
		}
		bzero(client, sizeof(t_client));
		/*---accept a connection (creating a data pipe)---*/
		printf("waiting for connection...\n");
		client->clientfd = accept(g_socket_sd, (struct sockaddr*)&(client->client_addr), &len);
		pthread_create(&tid, NULL, got_a_client, (void*)client);
	}
}