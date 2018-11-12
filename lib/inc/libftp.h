#ifndef LIBFTP_H
#define LIBFTP_H

#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>

#define SOCKET_MAX_SIZE 1300
#define PAYLOAD_MAX_SIZE 1200

typedef struct s_envelope
{
    char status;
    size_t pending_size;
    size_t payload_length;
    char payload[PAYLOAD_MAX_SIZE];
} t_envelope;

// int init_reponse(size_t length);
int send_response(int fd, t_envelope *envelope, char *buffer, size_t buffer_length);
void *ft_memcpy(void *dst, const void *src, size_t n);
size_t ft_strlen(const char *s);

#endif