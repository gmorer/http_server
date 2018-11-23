#include "ftp_client.h"

#define FT_P_CONNECT_FIRST_METHOD 1
#define FT_P_CONNECT_SECOND_METHOD 2

#define FT_P_CONNECT_METHOD FT_P_CONNECT_SECOND_METHOD

#if FT_P_CONNECT_METHOD == FT_P_CONNECT_FIRST_METHOD
static int first_method(int sock, char *address, int port)
{
    struct addrinfo *addr_info;
    struct sockaddr_in  client;

(void)port;
    if (getaddrinfo(address, NULL, NULL, &addr_info) != 0)
        return (0);
    while (addr_info)
	{
        ft_memcpy(&client, addr_info->ai_addr, sizeof(addr_info));
        if (port)
            client.sin_port = htons(port);
	    client.sin_family = AF_INET;
		printf("IP: %s\n", inet_ntoa(client.sin_addr));
        if (connect(sock, (struct sockaddr *)&client, sizeof(client)) == -1) {
            perror("connect");
            addr_info = addr_info->ai_next;
            continue ;
        }
        return (1);
	}
    return (0);
}
#endif

#if FT_P_CONNECT_METHOD == FT_P_CONNECT_SECOND_METHOD
static int second_method(int sock, char *address, int port)
{
    struct hostent *server;
    struct sockaddr_in  client;

    server = gethostbyname(address);
    if (!server)
        printf("gethostbyname error");
    client.sin_port = htons(port);
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = *((unsigned long *)server->h_addr);
    if (connect(sock, (struct sockaddr *)&client, sizeof(client)) == -1)
	{
        perror("connect");
        return (0);
	}
    return (1);

}
#endif

int connect_to_server(int sock, char *address, int port)
{
#if FT_P_CONNECT_METHOD == FT_P_CONNECT_FIRST_METHOD
        return first_method(sock, address, port);
#elif FT_P_CONNECT_METHOD == FT_P_CONNECT_SECOND_METHOD
        return second_method(sock, address, port);
#else
        return first_method(sock, address, port);
#endif
}