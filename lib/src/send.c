#include "libftp.h"

#define RST 1
#define SET 2
#define GET 3

/*
    START: setting total_size;
    WHILE: sending data;
    ENDL: oke
*/


size_t get_envelope_size(size_t payload_size)
{
    static const size_t struct_size = sizeof(t_envelope) - sizeof(char) * PAYLOAD_MAX_SIZE;
    return struct_size + (sizeof(char) * payload_size);
}


int ask_server(int sock, int status, char *payload, size_t payload_len)
{
    t_envelope request;
    char       buff[SOCKET_MAX_SIZE];
    size_t     res_len;

    if (payload_len > PAYLOAD_MAX_SIZE)
        return (0);
    ft_memset(&request, 0, sizeof(request));
    request.status = status;
    if (!payload)
        payload_len = 0;
    if (payload_len)
        ft_memcpy(request.payload, payload, payload_len);
    if (send(sock, &request, get_envelope_size(payload_len), 0) == -1)
    {
        write(2, "Can't send this request.\n", 25);
        return (0);
    }
    // TODO multiple socket response
    res_len = recv(sock, buff, SOCKET_MAX_SIZE, 0);
    fill_envelope(buff, &request);
    write(1, request.payload, request.payload_size);
    if (request.payload[request.payload_size - 1] != '\n')
        write(1, "\n", 1);
    return (1);
}

int send_response_in_loop(int fd, t_envelope *envelope, char *buffer, size_t buffer_length)
{
    size_t min_value = buffer_length > PAYLOAD_MAX_SIZE ? PAYLOAD_MAX_SIZE : buffer_length; // should always be buffer_length
    size_t response_length;

    printf("Sending one chunk of %zu byte(s).\n", buffer_length);
    response_length = get_envelope_size(min_value);
    ft_memcpy(envelope->payload, buffer, min_value);
    if (buffer_length > envelope->pending_size)
        printf("That should not happen, we send more in than the old pending size");
    envelope->pending_size -= buffer_length; // should never negativ
    envelope->status = 20;
    if (send(fd, envelope, response_length, 0) == -1)
    {
        printf("Error cantsend to the socket");
        return (0);
    }
    return (1);
}

int send_response(int fd, char *buffer, size_t buffer_length)
{
    t_envelope envelope;
    size_t index;
    size_t copied;
    size_t response_length;

    index = 0;
    printf("sending %zu bytes\n", buffer_length);
    while (index < buffer_length)
    {
        copied = buffer_length - index < PAYLOAD_MAX_SIZE ? buffer_length - index : PAYLOAD_MAX_SIZE;
        ft_memcpy(envelope.payload, buffer + index, copied);
        envelope.payload_size = copied;
        envelope.pending_size = buffer_length - (index + copied); // should be zero for the last one
        envelope.status = 20;
        response_length = get_envelope_size(copied);
        printf("payload length: %zu\n", envelope.payload_size);
        if (send(fd, &envelope, response_length, 0) == -1)
        {
            printf("Error cantsend to the socket");
            return (0);
        }
        index += copied;
        printf("index: %zu, buffer_length: %zu\n", index, buffer_length);
    }
    printf("returned after sending");
    return (1);
}