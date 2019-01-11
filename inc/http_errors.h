#ifndef HTTP_ERROR_H
# define HTTP_ERROR_H

typedef enum {
	BAD_REQUEST = 400,
	LENGTH_REQUIRED = 411,
	PAYLOAD_TOO_LARGE = 413,
	URI_TOO_LONG = 414,
	SERVER_ERROR = 500,
	HTTP_VERSION_NOT_SUPPORTED = 505,
	INSUFFICIENT_STORAGE = 507, // alloc error
}	http_erros;

void send_error(int fd, int error);

#endif