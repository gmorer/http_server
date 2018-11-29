#include "../inc/iosocket.h"

#define PORT 8080

char	g_root_path[PATH_MAX];
int g_socket_fd;

int main(void)
{
	unsigned int addrlen;
	struct sockaddr_in server;
	int true;
	pthread_t tid;
	t_client *client;

	if ((g_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (1);
	catch_sig();
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;

	getcwd(g_root_path, PATH_MAX);
	/*---Assign a port number to the socket---*/
	if (bind(g_socket_fd, (struct sockaddr *)&server, sizeof(server)) != 0)
	{
		perror("socket--bind");
		exit(1);
	}

	/*---Dunno whats this is ---*/
	true = 1;
	if (setsockopt(g_socket_fd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(g_socket_fd)) == -1)
	{
		perror("Setsockopt");
		exit(1);
	}

	/*---Make it a "listening socket"---*/
	if (listen(g_socket_fd, 20) != 0)
	{
		perror("socket--listen");
		exit(1);
	}
	/*---Forever... ---*/
	addrlen = sizeof(struct sockaddr_in);
	while (1)
	{
		client = malloc(sizeof(t_client));
		ft_memset(client, 0, sizeof(client));

		/*---accept a connection (creating a data pipe)---*/
		printf("Listening...\n");
		client->clientfd = accept(g_socket_fd, (struct sockaddr *)&(client->client_addr), &addrlen);
		pthread_create(&tid, NULL, got_a_client, (void *)client);
	}

	/*---Clean up (should never get here!)---*/
	close(g_socket_fd);
	return (1);
}
