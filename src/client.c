#include "../inc/iosocket.h"

#define BUFF_SIZE 4096

int g_socket_fd;

static void *response(void *arg)
{
	t_client *client;
	char	*res = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello world!";

	client = (t_client*)arg;
	// PARSE request (http_parser?)
	// OPEN asked file
	// SEND into while (for the socket max size)
	// use sendFile instead of send
	if (send(client->clientfd, res, strlen(res), 0) == -1)
		printf("cant send\n");
	return (NULL);
}

void	*got_a_client(void *arg)
{
	t_client	*client;
	pthread_t	tid;
	char	buffer[BUFF_SIZE];

	client = (t_client*)arg;
	printf("%s:%d connected\n", inet_ntoa(client->client_addr.sin_addr), ntohs(client->client_addr.sin_port));
	while ((client->req_len =  recv(client->clientfd, buffer, BUFF_SIZE, 0)))
	{
		printf("buffer:\n%s, len: %ld\n", buffer, client->req_len);
		pthread_create(&tid, NULL, response, (void*)client);
	}
	close(client->clientfd);
	printf("Client disconnected!");
	free(client);
	return (NULL);
}