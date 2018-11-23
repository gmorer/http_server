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
	char buffer[3];
	int ftp_code;

	buffer[0] = client->buffer[0];
	buffer[1] = client->buffer[1];
	buffer[2] = '\0';
	ftp_code = atoi(buffer);
	if (ftp_code == 0 || ftp_code >= COMMAND_MAX ||
		(client->req_len > 2 && (client->buffer[2] >= '0' && client->buffer[2] <= '9')))
		return invalid_command(client);
	printf("ftp_code: %s\n%s\n", buffer, client->buffer);
	return command_dispatcher(ftp_code, client);
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