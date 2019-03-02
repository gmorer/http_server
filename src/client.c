#include "server.h"

#define HEADER "HTTP/1.1 200 OK\r\nContent-Length: %zu\r\n\r\n"

int g_socket_sd;

static void free_inside_client(t_client *client)
{
	int					i;
	static const int	saved_fields_size = sizeof(client->clientfd) + sizeof(client->client_addr);

	i = 0;
	while (i < client->private.header_line)
	{
		if (client->private.headers[i].field)
			free(client->private.headers[i].field);
		if (client->private.headers[i].value)
			free(client->private.headers[i].value);
		i += 1;
	}
	if (client->params) free(client->params);
	if (client->url) free(client->url);
	if (client->private.buffer) free(client->private.buffer);
	// Do not bzero the first 2 fields of client (clientfd and client_addr)
	bzero((char*)client + saved_fields_size, sizeof(t_client) - saved_fields_size);
	return ;
}

static void *respond(t_client *client, t_response response)
{
	char	*header;
	int	i;

	i = 0;
	header = malloc(snprintf(NULL, 0, HEADER, response.body_len) + 1);
	sprintf(header, HEADER, response.body_len);
	write(1, "== NEW REQUEST ==\nurl: ", 23);
	write(1, client->url, strlen(client->url));
	write(1, "\n", 1);
	while (i++ < client->private.header_line)
		printf("%s : %s\n", client->private.headers[i].field, client->private.headers[i].value);
	write(1, "Body:\n", 6);
	write(1, client->body, client->body_len);
	printf("\nmethod: %d\n=====  END  =====\n", client->method);
	write(1, "== RESPONSE ==\nheader:\n", 23);
	write(1, header, strlen(header));
	write(1, "body:\n", 6);
	write(1, response.body, response.body_len);
	write(1, "\n== END ==\n", 11);
	if (send(client->clientfd, header, strlen(header), 0) == -1 ||
		send(client->clientfd, response.body, response.body_len, 0) == -1)
		dprintf(2, "Can't send\n");
	free(response.body);
	return (NULL);
}

int		got_a_message(t_client *client, http_parser *parser)
{
	ssize_t				nparsed;
	size_t				new_alloc_size;
	char				*content_length_str;
	size_t				content_length;
	char				*tmp;
	static const http_parser_settings	settings = {
		.on_url = url_callback,
		.on_header_field = header_field_callback,
		.on_header_value = header_value_callback,
		.on_body = body_callback,
		.on_message_begin = msg_begin_callback,
		.on_message_complete = msg_complet_callback,
		.on_headers_complete = header_complete_callback
	};

	(void)new_alloc_size;
	if (client->req_len == -1)
		return 0; // RECV ERROR
	content_length = 0;
	nparsed = http_parser_execute(parser, &settings, client->private.buffer, client->req_len);
	if (nparsed != client->req_len)
	{
		printf("Error while parsing http request.\n");
		send_error(client->clientfd, BAD_REQUEST);
		return 0; // HTPP PARSER ERROR
	}
	client->private.write_offset += nparsed;
	if (client->private.message_complete)
		return 1;
	if (!client->private.header_complete)
		new_alloc_size = client->private.buffer_size + BUFF_SIZE;
	else if ((content_length_str = get_header_value(client, "Content-Length")))
	{
		if (sscanf(content_length_str, "%zu", &content_length) != 1)
		{
			send_error(client->clientfd, BAD_REQUEST);
			return 0; // PARSING ERROR
		}
		new_alloc_size = client->body - client->private.buffer + content_length;
	}
	else
		new_alloc_size = client->private.buffer_size + BUFF_SIZE;
	if (new_alloc_size > MAX_REQUEST_SIZE)
	{
		send_error(client->clientfd, PAYLOAD_TOO_LARGE);
		return 0; // MESSAGE TO BIG
	}
	if (new_alloc_size > client->private.buffer_size)
	{
		if (!(tmp = realloc(client->private.buffer, new_alloc_size)))
		{
			ALLOCATION_ERROR;
			send_error(client->clientfd, INSUFFICIENT_STORAGE);
			return 0;
		}
		client->private.buffer_size = new_alloc_size;
	}
	if ((client->req_len = recv(client->clientfd, client->private.buffer + client->private.write_offset,
		MIN((size_t)BUFF_SIZE, client->private.buffer_size - client->private.write_offset), 0)))
		return got_a_message(client, parser);
	return 1; // socket close
}

void	*got_a_client(void *arg)
{
	t_client				*client;
	http_parser 			*parser;

	client = (t_client*)arg;
	parser = malloc(sizeof(http_parser));
	if (!parser)
	{
		ALLOCATION_ERROR;
		return NULL;
	}
	printf("sizeof client: %ld, sizeof parser: %ld\n", sizeof(t_client), sizeof(http_parser));
	http_parser_init(parser, HTTP_REQUEST);
	parser->data = client;
	if (!(client->private.buffer = malloc(BUFF_SIZE)))
	{
		ALLOCATION_ERROR;
		return NULL;
	}
	client->private.buffer_size = BUFF_SIZE;
	printf("%s:%d connected\n", inet_ntoa(client->client_addr.sin_addr), ntohs(client->client_addr.sin_port));
	while ((client->req_len = recv(client->clientfd, client->private.buffer, BUFF_SIZE, 0)))
	{
		if (got_a_message(client, parser))
			respond(client, execute_response(client, keep_endpoints(NULL)));
		free_inside_client(client);
		if (!(client->private.buffer = malloc(BUFF_SIZE)))
		{
			ALLOCATION_ERROR;
			return NULL;
		}
	}
	free(client->private.buffer);
	close(client->clientfd);
	printf("Client disconnected!");
	free(client);
	return (NULL);
}
