#ifndef IOSOCKET_H
#define IOSOCKET_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include "http_parser.h"
#include "http_method.h"

# define BUFF_SIZE 80*1024
# define MAX_HEADER 100
# define ALLOCATION_ERROR (write(2, "Allocation error\n", 17))

struct header_input {
	size_t	field_len;
	char	*field;
	size_t	value_len;
	char	*value;
};

struct private {
	char				last_was_value;
	int					header_line;
	struct header_input headers[MAX_HEADER];
	char				buffer[BUFF_SIZE];
};

typedef struct			s_client
{
	int					clientfd;
	struct	sockaddr_in	client_addr;
	size_t				req_len;
	char				*buffer;
	const char			*url;
	const char			*body;
	size_t				body_len;
	struct private		private;
	unsigned int		method;
}						t_client;

extern int g_socket_sd;

void catch_sig(void);
void *got_a_client(void *arg);

/* Parser callback */
int url_callback(http_parser *parser, const char *at, size_t length);
int header_field_callback(http_parser *parser, const char *at, size_t length);
int header_value_callback(http_parser *parser, const char *at, size_t length);
int body_callback(http_parser *parser, const char *at, size_t length);
int	header_complet_callback(http_parser *parser);

#endif