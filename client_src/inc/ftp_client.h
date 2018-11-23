#ifndef FTP_CLIENT_H
#define FTP_CLIENT_H

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "libftp.h"

int connect_to_server(int sock, char *address, int port);

#endif
