#include "server.h"

#define PORT 8080

int g_socket_sd;

int main(void)
{
	unsigned int 			addrlen;
	struct sockaddr_in	server;
	int								true;
	pthread_t					tid;
	t_client					*client;

	if ((g_socket_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		return (1);
	catch_sig();
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;

		/*---Assign a port number to the socket---*/
    if ( bind(g_socket_sd, (struct sockaddr*)&server, sizeof(server)) != 0 )
	{
		perror("socket--bind");
		exit(1);
	}

	/*---Dunno whats this is ---*/
	true = 1;
	if (setsockopt(g_socket_sd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(g_socket_sd)) == -1)
	{
    	perror("Setsockopt");
    	exit(1);
	}

	/*---Make it a "listening socket"---*/
	if ( listen(g_socket_sd, 20) != 0 )
	{
		perror("socket--listen");
		exit(1);
	}
		/*---Forever... ---*/
	addrlen = sizeof(struct sockaddr_in);
	while (1)
	{
		client = malloc(sizeof(t_client));
		if (!client)
		{
			ALLOCATION_ERROR;
			continue ;
		}
		bzero(client, sizeof(t_client));

		/*---accept a connection (creating a data pipe)---*/
		printf("waiting for connection...\n");
		client->clientfd = accept(g_socket_sd, (struct sockaddr*)&(client->client_addr), &addrlen);
		pthread_create(&tid, NULL, got_a_client, (void*)client);
	}

	/*---Clean up (should never get here!)---*/
	close(g_socket_sd);
	return (1);
}
