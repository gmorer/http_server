#ifndef IOSOCKET_H
#define IOSOCKET_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr

typedef struct	s_client
{
	int clientfd;
	struct sockaddr_in client_addr;
	size_t req_len;
}			t_client;

extern int g_socket_fd;

void catch_sig(void);

#endif