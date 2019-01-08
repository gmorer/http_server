#include "server.h"

#define HEADER "HTTP/1.1 200 OK\r\nContent-Length: %zu\r\n\r\n"

int g_socket_sd;

static void *respond(t_client *client)
{
	char	body[] = "Hello world!";
	char	*header;
	int		i;
	size_t	body_len;

	i = 0;
	body_len = strlen(body);
	header = malloc(snprintf(NULL, 0, HEADER, body_len) + 1);
	sprintf(header, HEADER, body_len);
	write(1, "== NEW REQUEST ==\n", 18);
	write(1, "url: ", 5);
	write(1, client->url, strlen(client->url));
	write(1, "\n", 1);
	while (i++ < client->private.header_line)
		printf("%s : %s\n", client->private.headers[i].field, client->private.headers[i].value);
	write(1, "Body:\n", 6);
	write(1, client->body, client->body_len);
	write(1, "=====  END  =====\n", 18);
	if (send(client->clientfd, header, strlen(header), 0) == -1 ||
		send(client->clientfd, body, strlen(body), 0) == -1)
		printf("cant send\n");
	return (NULL);
}

void	*got_a_client(void *arg)
{
	t_client				*client;
	http_parser_settings	settings;
	http_parser 			*parser;
	size_t					nparsed;

	client = (t_client*)arg;
	parser = malloc(sizeof(http_parser));
	if (!parser)
	{
		ALLOCATION_ERROR;
		return NULL;
	}
	printf("sizeof client: %ld, sizeof parser: %ld\n", sizeof(t_client), sizeof(http_parser));
	settings.on_url = url_callback;
	settings.on_header_field = header_field_callback;
	settings.on_header_value = header_value_callback;
	settings.on_body = body_callback;
	//settings.on_header_field = header_callback;
	http_parser_init(parser, HTTP_REQUEST);
	parser->data = client;
	printf("%s:%d connected\n", inet_ntoa(client->client_addr.sin_addr), ntohs(client->client_addr.sin_port));
	while ((client->req_len = recv(client->clientfd, client->private.buffer, BUFF_SIZE, 0)))
	{
		nparsed = http_parser_execute(parser, &settings, client->private.buffer, client->req_len);
		if (nparsed != client->req_len)
			printf("Error while parsing http request.\n");
		respond(client);
	}
	close(client->clientfd);
	printf("Client disconnected!");
	free(client);
	return (NULL);
}