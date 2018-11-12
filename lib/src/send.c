#include "libftp.h"

#define RST 1
#define SET 2
#define GET 3

/*
    START: setting total_size;
    WHILE: sending data;
    ENDL: oke
*/

int send_response(int fd, t_envelope *envelope, char *buffer, size_t buffer_length)
{
    size_t min_value = buffer_length > PAYLOAD_MAX_SIZE ? PAYLOAD_MAX_SIZE : buffer_length; // should always be buffer_length
    size_t response_length;

    printf("Sending one chunk of %zu byte(s).\n", buffer_length);
    response_length = sizeof(char) + sizeof(size_t) * 2 + (sizeof(char) * min_value);
    ft_memcpy(envelope->payload, buffer, min_value);
    if (buffer_length > envelope->pending_size)
        printf("That should not happen, we send more in than the old pending size");
    envelope->pending_size -= buffer_length; // should never negativ
    if (!envelope->pending_size)
        envelope->status = 22;
    else
        envelope->status = 21;
    if (send(fd, envelope, response_length, 0) == -1)
    {
        printf("Error cantsend to the socket");
        return (0);
    }
    return (1);
}