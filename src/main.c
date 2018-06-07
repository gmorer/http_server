#include "../inc/iosocket.h"

#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <signal.h>

#define PORT 8080
#define BUFF_SIZE 4096

int g_socket_fd;

int main(void)
{
	struct sockaddr_in	server;
	char	buffer[BUFF_SIZE];
	int true;

	if ((g_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		return (1);
	catch_sig();
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;

		/*---Assign a port number to the socket---*/
    if ( bind(g_socket_fd, (struct sockaddr*)&server, sizeof(server)) != 0 )
	{
		perror("socket--bind");
		exit(1);
	}

	/*---Dunno whats this is ---*/
	true = 1;
	if (setsockopt(g_socket_fd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(g_socket_fd)) == -1)
	{
    	perror("Setsockopt");
    	exit(1);
	}

	/*---Make it a "listening socket"---*/
	if ( listen(g_socket_fd, 20) != 0 )
	{
		perror("socket--listen");
		exit(1);
	}
		/*---Forever... ---*/
	while (1)
	{
		int									clientfd;
		struct sockaddr_in	client_addr;
		unsigned int 			addrlen;
		size_t							req_len;
		
		addrlen = sizeof(client_addr);

		/*---accept a connection (creating a data pipe)---*/
		printf("waiting for connection...\n");
		clientfd = accept(g_socket_fd, (struct sockaddr*)&client_addr, &addrlen);
		printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		/*---Echo back anything sent---*/
		req_len =  recv(clientfd, buffer, BUFF_SIZE, 0);
		if (!req_len) {
			printf("Client disconnected!");
		}
		else {
			printf("buffer: %s, len: %ld\n", buffer, req_len);
			send(clientfd, buffer,  req_len, 0);
		}

		/*---Close data connection---*/
		//close(clientfd);
	}

	/*---Clean up (should never get here!)---*/
	close(g_socket_fd);
	return (1);
}
