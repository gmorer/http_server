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
# include "utils.h"
# include "http_errors.h"

# define BUFF_SIZE 80*1024
# define MAX_HEADER 100
# define ALLOCATION_ERROR (write(2, "Allocation error\n", 17))
# define MAX_REQUEST_SIZE 1024 * 1024 * 5 // 5 MegaBytes

struct header_input {
	size_t	field_len;
	char	*field;
	size_t	value_len;
	char	*value;
};

struct private {
	char				last_was_value;
	char				message_complete;
	char				header_complete;
	int					header_line;
	struct header_input headers[MAX_HEADER];
	size_t				write_offset;
	size_t				buffer_size;
	char				*buffer;
};

typedef struct			s_client
{
	int					clientfd;
	struct	sockaddr_in	client_addr;
	ssize_t				req_len;
	char				*buffer;
	char				*url;
	char				*body; // NEED TO REDO THAT SINCE I CAN REALLOC THE BUFFER
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
int msg_complet_callback(http_parser *parser);
int msg_begin_callback(http_parser *parser);
int header_complete_callback(http_parser *parser);
/* Utils functions */
char	*get_header_value(t_client *client, char *field);

#endif