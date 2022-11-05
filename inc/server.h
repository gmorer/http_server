#ifndef _SERVER_H_
#define _SERVER_H_

#include <arpa/inet.h>
#include <assert.h>
#include <regex.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "http_errors.h"
#include "http_method.h"
#include "http_parser.h"
#include "utils.h"
#include "worker.h"

# define POLL_BUFFER_LEN 50
# define BUFF_SIZE 80*1024
# define DEFAULT_PORT 8080
# define MAX_HEADER 100
# define ALLOCATION_ERROR (write(2, "Allocation error\n", 17))
# define MAX_REQUEST_SIZE 1024 * 1024 * 5 // 5 MegaBytes
# define ENDPOINTS_END ((t_endpoint){NULL, 0, 0, 0})

struct header_input {
	size_t	field_size;
	char	*field;
	size_t	value_size;
	char	*value;
};

enum _parser_state {
	INITIAL = 0,
	HEADER_FIELD,
	HEADER_VALUE,
};

struct _private {
	pthread_mutex_t 	lock;
	enum _parser_state  parser_state;
	char				last_was_value;
	char				message_complete;
	char				header_complete;
	size_t				write_offset;
	size_t				buffer_size;
	char				buffer[BUFF_SIZE];
};

typedef struct			s_client
{
	int					clientfd;
	struct	sockaddr_in	client_addr;
	ssize_t				req_len;
	size_t				url_size;
	char				*url;
	regmatch_t			*params;
	size_t				params_length;
	int					headers_len;
	struct header_input headers[MAX_HEADER];
	char				*body;
	size_t				body_size;
	struct _private		_private;
	unsigned int		method;
	http_parser			parser;
}						t_client;

typedef struct			s_response
{
	char				*body;
	size_t				body_len;
	int					http_code;
	struct header_input	*headers;
	int					headers_len;
}						t_response;

typedef t_response (endpoint_action)(t_client *client);

typedef struct		s_endpoint
{
	char			*url;
	size_t			args_no;
	endpoint_action	*action;
	regex_t			comp_url;
}					t_endpoint;

/* client.c */
t_client *accept_client(int socket_fd);

void catch_sig(void);

void close_server();
int	init_server(int port, t_endpoint *endpoints);
void launch_server(void);

/* client.c */
t_client *accept_client(int socket_fd);
void read_client(void *data);
void free_client(t_client *client);

/* Parser callback */
/* int url_callback(http_parser *parser, const char *at, size_t length); */
/* int header_field_callback(http_parser *parser, const char *at, size_t length); */
/* int header_value_callback(http_parser *parser, const char *at, size_t length); */
/* int body_callback(http_parser *parser, const char *at, size_t length); */
/* int msg_complet_callback(http_parser *parser); */
/* int msg_begin_callback(http_parser *parser); */
/* int header_complete_callback(http_parser *parser); */
/* regex functions */
bool		compil_regex(t_endpoint *endpoints);
t_response	execute_response(t_client *client);
/* Utils functions */
char	*get_header_value(t_client *client, char *field);



void panic(char *msg);

#endif
