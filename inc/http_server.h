/*
 * The purpose of this file is to be included
 * when compiling with the http_server library
 */

#ifndef HTTP_SERVER_H
# define HTTP_SERVER_H

# include <sys/types.h>
# include <regex.h>
# include <netinet/in.h>

# define MAX_HEADER 100
# define ENDPOINTS_END ((t_endpoint){NULL, 0, 0, 0})

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
	size_t				write_offset;
	size_t				buffer_size;
	char				*buffer;
};

typedef struct			s_client
{
	int					clientfd;
	struct	sockaddr_in	client_addr;
	ssize_t				req_len;
	char				*url;
	regmatch_t			*params;
	size_t				params_length;
	struct header_input headers[MAX_HEADER];
	int					header_len;
	char				*body;
	size_t				body_len;
	struct private		private;
	unsigned int		method;
}						t_client;

typedef struct			s_response
{
	char				*body;
	size_t				body_len;
	int					http_code;
	struct header_input	headers;
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

int		init_server(int port, t_endpoint *endpoints);
void	launch_server(void);

#endif
