#include "../inc/server.h"

// char *get_error(int error)
// {
// 	switch(error) {
// 		case BAD_REQUEST: return "Bad Request";
// 		case LENGTH_REQUIRED: return "Length Required";
// 		case PAYLOAD_TOO_LARGE: return "Payload Too Large";
// 		case URI_TOO_LONG: return "";
// 		case SERVER_ERROR: return "";
// 		case HTTP_VERSION_NOT_SUPPORTED: return "";
// 		case INSUFFICIENT_STORAGE: return "";
// 	}
// }

void send_error(int fd, int error)
{
	static const char	header[] = "HTTP/1.1 %d %s\r\n\r\n";
	const char			*error_str;
	char				*response;
	size_t				response_length;

	error_str = http_status_str(error);
	response_length = snprintf(NULL, 0, header, error, error_str);
	response = malloc(response_length);
	if (!response)
	{
		ALLOCATION_ERROR;
		send(fd, "HTTP/1.1 507 Insufficient Storage\r\n\r\n", 37, 0);
		return ;
	}
	sprintf(response, header, error, error_str);
	send(fd, response, response_length, 0);
	return ;
}
