#include "../inc/iosocket.h"
#include "../inc/command_array.h"

#define INVALID_RESPONSE "Invalid protocol, protocol in use on this server: FT_FTP1"
#define INVALID_RESPONSE_LEN 57

int g_socket_fd;

static void *invalid_command(t_client *client)
{
	send_failure(client->clientfd, INVALID_RESPONSE, INVALID_RESPONSE_LEN);
	return (NULL);
}

static void *command_dispatcher(int ftp_code, t_client *client)
{
	static const t_command_fun command[COMMAND_MAX] = INIT_ARRAY;

	return command[ftp_code](client);
}

static void *response(t_client *client)
{
	t_envelope *response;

	if (client->req_len < 5)
		return invalid_command(client);
	response = (t_envelope*)client->buffer;
	printf("response status: %d", (char)response->status);
	if (response->status < 0 || response->status > COMMAND_MAX)
		return invalid_command(client);
	return command_dispatcher(response->status, client);
}

void *got_a_client(void *arg)
{
	t_client *client;

	client = (t_client *)arg;
	printf("%s:%d connected\n", inet_ntoa(client->client_addr.sin_addr), ntohs(client->client_addr.sin_port));
	while ((client->req_len = recv(client->clientfd, client->buffer, BUFF_SIZE, 0)))
	{
		response(client);
	}
	close(client->clientfd);
	printf("Client disconnected!");
	free(client);
	return (NULL);
}