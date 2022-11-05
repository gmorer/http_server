#include "../inc/server.h"

#define HEADER_HTTP "HTTP/1.1 "
#define HEADER "HTTP/1.1 200 OK\r\nContent-Length: %zu\r\n\r\n"


int on_header_field_cb(http_parser* parser, const char *data, size_t length) {
	// TODO rework to hashmap
	t_client *client = (t_client*)parser->data;

	if (client->_private.parser_state == HEADER_VALUE) {
		if (client->headers_len == MAX_HEADER - 1) {
			return 1;
		}
		client->headers_len += 1;
	}

	size_t previous_size = client->headers[client->headers_len].field_size;
	char *previous_data = client->headers[client->headers_len].field;

	printf("on header field cb alloc ptr: %p, length: %ld\n", previous_data, previous_size + length);
	char *field = realloc(previous_data, previous_size + length + 1);
	memcpy(field + previous_size, data, length);
	field[previous_size + length] = 0;
	client->headers[client->headers_len].field = field;
	client->headers[client->headers_len].field_size += length;
	client->_private.parser_state = HEADER_FIELD;
	return 0;
}

int on_header_value_cb(http_parser* parser, const char *data, size_t length) {
	// TODO rework to hashmap
	t_client *client = (t_client*)parser->data;

	size_t previous_size = client->headers[client->headers_len].value_size;
	char *previous_data = client->headers[client->headers_len].value;

	printf("on header value cb alloc\n");
	char *value = realloc(previous_data, previous_size + length + 1);
	memcpy(value + previous_size, data, length);
	value[previous_size + length] = 0;
	client->headers[client->headers_len].value = value;
	client->headers[client->headers_len].value_size += length;
	client->_private.parser_state = HEADER_VALUE;
	return 0;
}

int on_url_cb(http_parser* parser, const char *data, size_t length) {
	t_client *client = (t_client*)parser->data;

	size_t previous_size = client->url_size;
	char *previous_data = client->url;

	printf("on url cb alloc, url: %s\n", data);
	char *value = realloc(previous_data, previous_size + length + 1);
	memcpy(value + previous_size, data, length);
	value[previous_size + length] = 0;
	client->url = value;
	client->url_size += length;
	return 0;
}

int on_body_cb(http_parser* parser, const char *data, size_t length) {
	t_client *client = (t_client*)parser->data;

	size_t previous_size = client->body_size;
	char *previous_data = client->body;

	printf("on body cb alloc\n");
	char *value = realloc(previous_data, previous_size + length + 1);
	memcpy(value + previous_size, data, length);
	value[previous_size + length] = 0;
	client->body = value;
	client->body_size += length;
	return 0;
}

void respond(void *data)
{
	char	*header;
	int		i = 0;
	t_client *client = (t_client*)data;

	t_response response = execute_response(client);
	header = malloc(snprintf(NULL, 0, HEADER, response.body_len) + 1);
	sprintf(header, HEADER, response.body_len);
	printf("== NEW REQUEST ==\n");
	printf("url: %s\n", client->url);
	while (i < client->headers_len) {
		printf("%s : %s\n", client->headers[i].field, client->headers[i].value);
		i += 1;
	}
	if (client->body != NULL) {
		printf("Body:\n");
		printf("%s\n", client->body);
	}
	printf("method: %d\n", client->method);
	printf("=====  END  =====\n");
	printf("== RESPONSE ==\n");
	printf("header:\n");
	printf("%s", header);
	printf("body[%zu]:\n", response.body_len);
	printf("%s\n", response.body);
	printf("== END ==\n");
	if (send(client->clientfd, header, strlen(header), 0) == -1 ||
		send(client->clientfd, response.body, response.body_len, 0) == -1)
		dprintf(2, "Can't send\n");
	free(response.body);
	free(header);
}

int on_message_complete_cb(http_parser *parser) {
	respond(parser->data);
	return 0;
}

int on_message_begin_cb(http_parser *parser) {
	UNUSED(parser);
	printf("On message beginn\n");
	return 0;
}
int on_headers_complete_cb(http_parser *parser) {
	UNUSED(parser);
	printf("On headers complete\n");
	return 0;
}

t_client *accept_client(int socket_fd) {
	socklen_t len;
	printf("calloc acept client\n");
	t_client *client = calloc(1, sizeof(*client));

	if (client == NULL) {
		return NULL;
	}
	len = sizeof(client->client_addr);
	client->clientfd = accept(socket_fd, (struct sockaddr *) &(client->client_addr), &len);

	if (client->clientfd == -1) {
		dprintf(2, "Wasnt hable to accept client\n");
		free(client);
		return NULL;
	}

	http_parser_init(&client->parser, HTTP_REQUEST);
	client->parser.data = client;
	client->_private.parser_state = INITIAL;
	pthread_mutex_init(&client->_private.lock, NULL);
	client->headers_len = 0;

	/* TODO: not reading since writing is not async yet
	int flags = fcntl(client->clientfd, F_GETFL, 0);
	fcntl(client->clientfd, F_SETFL, flags | O_NONBLOCK);
	*/

	return client;
}

void read_client(void *data) {
	t_client *client = (t_client*)data;

	static const http_parser_settings settings = {
		.on_url = on_url_cb,
		.on_header_field = on_header_field_cb,
		.on_header_value = on_header_value_cb,
		.on_body = on_body_cb,
		.on_message_begin = on_message_begin_cb,
		.on_headers_complete = on_headers_complete_cb,
		.on_message_complete = on_message_complete_cb
	};

	pthread_mutex_lock(&client->_private.lock);
	printf("sizeof buffer: %ld\n", sizeof(client->_private.buffer));
	ssize_t length = recv(
		client->clientfd,
		client->_private.buffer,
		sizeof(client->_private.buffer),
		0
	);
	if (length == -1) {
		pthread_mutex_unlock(&client->_private.lock);
		printf("recv return an error\n");
		/* return false; */
		return ;
	}
	ssize_t nparsed = http_parser_execute(
		&client->parser,
		&settings,
		client->_private.buffer,
		length
	);

	if (client->parser.upgrade) {
	  /* handle new protocol */
		printf("pls handle upgrtade\n");
	} else if (nparsed != length) {
	  /* Handle error. Usually just close the connection. */
		printf("gonna free the clkient\n");
		free_client(client);
		/* return true; */
		return ;
	}
	pthread_mutex_unlock(&client->_private.lock);
	/* return true; */
	return ;
}

void free_client(t_client *client)
{
	static const int saved_fields_size = sizeof(client->clientfd) + sizeof(client->client_addr);

	while (client->headers_len >= 0) {
		free(client->headers[client->headers_len].field);
		if (client->headers[client->headers_len].value) {
			free(client->headers[client->headers_len].value);
		}
		client->headers_len -= 1;
	}
	close(client->clientfd);
	if (client->params) free(client->params);
	if (client->url) free(client->url);
	// Do not bzero the first 2 fields of client (clientfd and client_addr)
	bzero((char*)client + saved_fields_size, sizeof(t_client) - saved_fields_size);
	pthread_mutex_t lock = client->_private.lock;
	free(client);
	pthread_mutex_unlock(&lock);
	return ;
}
/*

int		got_a_message(t_client *client, http_parser *parser)
{
	ssize_t				nparsed;
	size_t				new_alloc_size;
	char				*content_length_str;
	size_t				content_length;
	char				*tmp;

	(void)new_alloc_size;
	if (client->req_len == -1)
		return 0; // RECV ERROR
	content_length = 0;
	nparsed = http_parser_execute(parser, &settings, client->_private.buffer, client->req_len);
	if (nparsed != client->req_len)
	{
		printf("Error while parsing http request.\n");
		send_error(client->clientfd, BAD_REQUEST);
		return 0; // HTPP PARSER ERROR
	}
	client->_private.write_offset += nparsed;
	if (client->_private.message_complete)
		return 1;
	if (!client->_private.header_complete)
		new_alloc_size = client->_private.buffer_size + BUFF_SIZE;
	else if ((content_length_str = get_header_value(client, "Content-Length")))
	{
		if (sscanf(content_length_str, "%zu", &content_length) != 1)
		{
			send_error(client->clientfd, BAD_REQUEST);
			return 0; // PARSING ERROR
		}
		new_alloc_size = client->body - client->_private.buffer + content_length;
	}
	else
		new_alloc_size = client->_private.buffer_size + BUFF_SIZE;
	if (new_alloc_size > MAX_REQUEST_SIZE)
	{
		send_error(client->clientfd, PAYLOAD_TOO_LARGE);
		return 0; // MESSAGE TO BIG
	}
	if (new_alloc_size > client->_private.buffer_size)
	{
		if (!(tmp = realloc(client->_private.buffer, new_alloc_size)))
		{
			ALLOCATION_ERROR;
			send_error(client->clientfd, INSUFFICIENT_STORAGE);
			return 0;
		}
		client->_private.buffer_size = new_alloc_size;
	}
	if ((client->req_len = recv(client->clientfd, client->_private.buffer + client->_private.write_offset,
		MIN((size_t)BUFF_SIZE, client->_private.buffer_size - client->_private.write_offset), 0)))
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
	if (!(client->_private.buffer = calloc(1, BUFF_SIZE)))
	{
		ALLOCATION_ERROR;
		return NULL;
	}
	client->_private.buffer_size = BUFF_SIZE;
	printf("%s:%d connected\n", inet_ntoa(client->client_addr.sin_addr), ntohs(client->client_addr.sin_port));
	while ((client->req_len = recv(client->clientfd, client->_private.buffer, BUFF_SIZE, 0)))
	{
		if (got_a_message(client, parser))
			respond(client, execute_response(client));
		free_inside_client(client);
		if (!(client->_private.buffer = calloc(1, BUFF_SIZE)))
		{
			ALLOCATION_ERROR;
			return NULL;
		}
	}
	free(client->_private.buffer);
	close(client->clientfd);
	printf("Client disconnected!");
	free(client);
	return (NULL);
}
*/
