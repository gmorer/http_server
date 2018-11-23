#include "ftp_client.h"

int main(int argc, char **argv)
{
	int sock;
	int port;
	char *hostname;

	if (argc < 3)
	{
		printf("Usage: clientr [hostname] [PORT]\n");
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
	else
		printf("Cant connect :(\n");
	/*
	printf("return: %d\n", getaddrinfo(argv[1], NULL, NULL, &addr_info));
	printf("IP: %s\n", inet_ntoa(((struct sockaddr_in *)(addr_info->ai_addr))->sin_addr));
	while (addr_info->ai_next)
	{
		addr_info = addr_info->ai_next;
		printf("IP: %s\n", inet_ntoa(((struct sockaddr_in *)(addr_info->ai_addr))->sin_addr));
	}
	return 1;
	printf("%s\n", server->h_addr_list[0]);
	printf("%s\n", server->h_name);
	ip = (struct in_addr *)server->h_addr_list[0];
	printf("%s\n", inet_ntoa(*ip));
	ft_memset(&client, '0', sizeof(struct sockaddr_in));
	// client.sin_addr = (struct in_addr *)server->h_addr_list[0];
	ft_memcpy(&(client.sin_addr), server->h_addr_list[0], sizeof(struct in_addr));
	client.sin_port = htons(port);
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr(server->h_name);
	// if (inet_pton(AF_INET, argv[1], &client.sin_addr) <= 0)
	// {
	// 	printf("\nInvalid address/ Address not supported \n");
	// 	return -1;
	// }

	if (connect(sock, (struct sockaddr *)&client, sizeof(client)) == -1)
	{
		printf("Cant connect to %s.\n", argv[1]);
		exit(1);
	}
	send(sock, "hello", 5, 0);
	printf("Hellow world!\n");
	*/
	return 0;
}
