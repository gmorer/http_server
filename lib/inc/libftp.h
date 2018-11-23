#ifndef LIBFTP_H
#define LIBFTP_H

#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define SOCKET_MAX_SIZE 1300
#define PAYLOAD_MAX_SIZE 1200

typedef struct s_envelope
{
    char status;
    size_t pending_size;
    size_t payload_size;
    char payload[PAYLOAD_MAX_SIZE];
} t_envelope;

// int init_reponse(size_t length);
int send_response_in_loop(int fd, t_envelope *envelope, char *buffer, size_t buffer_length);
int send_response(int fd, char *buffer, size_t buffer_length);
void *ft_memcpy(void *dst, const void *src, size_t n);
int send_failure(int fd, char *message, size_t length);
int send_success(int fd, char *message, size_t length);
size_t ft_strlen(const char *s);
char *ft_strncpy(char *dst, const char *src, size_t n);
size_t get_envelope_size(size_t payload_size);
void *ft_memset(void *b, int c, size_t len);
int ft_atoi(const char *str);

#endif