#ifndef IOSOCKET_H
#define IOSOCKET_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <dirent.h>

#define BUFF_SIZE 4096
#define SOCKET_MAX_SIZE 1300
#define COMMAND_MAX 50

// Les constantes de préprocesseur (#define) que vous créez ne doivent être utilisés
// que pour associer des valeurs littérales et constantes, et rien d’autre.

/*
	My own FTP:

	3 digit number

	incomming ->

	10 FT_FTP1 // To start the connection and say wich version of FT_FTP you are using
	21 ls
	22 cd
	23 pwd
	31 FILENAME // to send the file to the client
	41 FILENAME DATA_LENGTH DATA // to receive the file
	11 // To close the connection.
*/

typedef struct s_client
{
	int clientfd;
	struct sockaddr_in client_addr;
	size_t req_len;
	char buffer[BUFF_SIZE];
} t_client;

typedef void *(*t_command_fun)(t_client *client);

extern int g_socket_fd;

size_t ft_strlen(const char *s);
void *command_start(t_client *client);
void *command_close(t_client *client);
void *command_ls(t_client *client);
void *command_cd(t_client *client);
void *command_pwd(t_client *client);
void *command_send(t_client *client);
void *command_receive(t_client *client);
void catch_sig(void);
void *got_a_client(void *arg);

#endif