#include "server.h"

#define HEADER_LINE (client->private.header_line)
#define CURRENT_LINE (client->private.headers[HEADER_LINE - 0])

int url_callback(http_parser *parser, const char *at, size_t length)
{
	t_client *client;

	client = parser->data;
	client->url = strndup(at, length);
	return 0;
}

int	header_complet_callback(http_parser *parser)
{
	t_client *client;

	client = parser->data;
	client->method = parser->method;
	return 0;
}

/*
 * Parsing the header, from the example: https://gist.github.com/ry/155877
 */

int header_field_callback(http_parser *parser, const char *at, size_t length)
{
	t_client *client;

	client = parser->data;
	if (HEADER_LINE >= MAX_HEADER)
			return 1; // error
	if (!client->private.last_was_value && client->private.headers[0].field != NULL)
	{
    	CURRENT_LINE.field = realloc(CURRENT_LINE.field,
        	CURRENT_LINE.field_len + length + 1);
		memcpy(CURRENT_LINE.field + CURRENT_LINE.field_len, at, length);
		CURRENT_LINE.field_len += length;
	} else {
		HEADER_LINE++;
		if (HEADER_LINE >= MAX_HEADER)
			return 1; // error
		CURRENT_LINE.field_len = length;
		CURRENT_LINE.field = strndup(at, length);
	}
	CURRENT_LINE.field[CURRENT_LINE.field_len] = '\0';
	client->private.last_was_value = 0;
	return 0;
}

int header_value_callback(http_parser *parser, const char *at, size_t length)
{
	t_client *client;

	client = parser->data;
	if (HEADER_LINE >= MAX_HEADER)
			return 1; // error
	if (CURRENT_LINE.value == NULL)
	{
		CURRENT_LINE.value_len = length;
		CURRENT_LINE.value = strndup(at, length);
	} else {
    	CURRENT_LINE.value = realloc(CURRENT_LINE.value,
        	CURRENT_LINE.value_len + length + 1);
		memcpy(CURRENT_LINE.value + CURRENT_LINE.value_len, at, length);
		CURRENT_LINE.value_len += length;
	}
	CURRENT_LINE.value[CURRENT_LINE.value_len] = '\0';
	client->private.last_was_value = 1;
	return 0;
}

int body_callback(http_parser *parser, const char *at, size_t length)
{
	t_client *client;

	client = parser->data;
	client->body = (char*)(at - client->private.buffer); // just get the offset if I ave to realloc  after
	client->body_len = length;
	client->private.buffer[at - client->private.buffer + length] = '\0';
	return 0;
}

int msg_begin_callback(http_parser *parser)
{
	t_client *client;

	client = parser->data;
	client->private.message_complete = 0;
	client->private.header_complete = 0;
	//write(1, "\n\n\n\n\nBEGIN\n\n\n\n\n", 15);
	return 0;
}

int msg_complet_callback(http_parser *parser)
{
	t_client *client;

	client = parser->data;
	client->private.message_complete = 1;
	client->body = client->private.buffer + (unsigned long)client->body;
	//write(1, "\n\n\n\n\nEND\n\n\n\n\n", 13);
	return 0;
}

int header_complete_callback(http_parser *parser)
{
	t_client *client;

	client = parser->data;
	client->private.header_complete = 1;
	return 0;
}
