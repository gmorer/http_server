#include "ftp_client.h"

int main(int argc, char **argv)
{
	int sock;
	int port;
	char *hostname;

	if (argc < 3)
	{
		printf("Usage: client [hostname] [PORT]\n");
		exit(1);
	}
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}
	hostname = argv[1];
	port = ft_atoi(argv[2]);
	if (!port || port < 0)
	{
		printf("Invalid port.\n");
		exit(1);
	}

	if (connect_to_server(sock, argv[1], port))
    	send(sock, "hello", 5, 0);
	else {
		return (0);
		printf("Cant connect :(\n");
	}
	main_loop(sock);
	return 0;
}
