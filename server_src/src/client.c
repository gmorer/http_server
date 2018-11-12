#include "../inc/iosocket.h"
#include "../inc/command_array.h"

#define INVALID_RESPONSE "Invalid protocol, protocol in use on this server: FT_FTP1"
#define INVALID_RESPONSE_LEN 57

int g_socket_fd;

static void *invalid_command(t_client *client)
{
	if (send(client->clientfd, INVALID_RESPONSE, INVALID_RESPONSE_LEN, 0) == -1)
		printf("cant send\n");
	return (NULL);
}

static void *invalid_response(void *arg)
{
	t_client *client;

	client = (t_client *)arg;
	if (send(client->clientfd, INVALID_RESPONSE, INVALID_RESPONSE_LEN, 0) == -1)
		printf("cant send\n");
	return (NULL);
}

static void *command_dispatcher(int ftp_code, t_client *client)
{
	static const t_command_fun command[COMMAND_MAX] = INIT_ARRAY;

	return command[ftp_code](client);
}

static void *response(void *arg)
{
	t_client *client;
	char buffer[3];
	// char *res = "Yep";
	int ftp_code;

	client = (t_client *)arg;
	buffer[0] = client->buffer[0];
	buffer[1] = client->buffer[1];
	buffer[2] = '\0';
	ftp_code = atoi(buffer);
	if (ftp_code == 0 || ftp_code >= COMMAND_MAX || (client->req_len > 2 && (client->buffer[2] >= '0' && client->buffer[2] <= '9')))
		return invalid_response(arg);
	printf("ftp_code: %s\n%s\n", buffer, client->buffer);
	return command_dispatcher(ftp_code, client);
}

void *got_a_client(void *arg)
{
	t_client *client;
	pthread_t tid;

	client = (t_client *)arg;
	printf("%s:%d connected\n", inet_ntoa(client->client_addr.sin_addr), ntohs(client->client_addr.sin_port));
	while ((client->req_len = recv(client->clientfd, client->buffer, BUFF_SIZE, 0)))
	{
		if (client->req_len < 2) // INVALID REQUEST
			pthread_create(&tid, NULL, invalid_response, (void *)client);
		else
			pthread_create(&tid, NULL, response, (void *)client);
		// printf("buffer:\n%s, len: %ld\n", buffer, client->req_len);
	}
	close(client->clientfd);
	printf("Client disconnected!");
	free(client);
	return (NULL);
}