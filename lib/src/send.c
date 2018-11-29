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
    t_envelope *request;
    char       buff[sizeof(t_envelope)];
    size_t     res_len;

    request = (t_envelope*)buff;
    fflush(stdout);
    if (payload_len > PAYLOAD_MAX_SIZE)
        return (0);
    ft_memset(request, 0, sizeof(t_envelope));
    request->status = status;
    request->payload_size = payload_len;
    if (!payload)
        payload_len = 0;
    if (payload_len)
        ft_memcpy(request->payload, payload, payload_len);
    if (send(sock, request, get_envelope_size(payload_len), 0) == -1)
    {
        write(2, "Can't send this request.\n", 25);
        return (0);
    }
    // TODO multiple socket response
    res_len = recv(sock, buff, sizeof(t_envelope), 0);
    // fill_envelope(buff, &request);
    write(1, request->payload, request->payload_size);
    if (request->payload[request->payload_size - 1] != '\n')
        write(1, "\n", 1);
    return (1);
}

int send_multiple_response(int fd, t_envelope envelope, size_t actual_size, size_t pending_size)
{
    size_t      response_length;

    // printf("sending %zu bytes\n", actual_size);
    envelope.payload_size = actual_size;
    envelope.pending_size = pending_size;
    envelope.status = 21;
    response_length = get_envelope_size(actual_size);
    // write(1, "sending payload........\n", 24);
    if (send(fd, &envelope, response_length, 0) == -1)
        return (0);
    // write(1, "waiting for response...\n", 24);
    if (recv(fd, &envelope, sizeof(t_envelope), 0) == -1)
        return (0);
    // write(1, "RESPONSE Ok\n", 12);
    return (1);
}

int send_response(int fd, char *buffer, size_t buffer_length)
{
    t_envelope envelope;
    size_t index;
    size_t copied;
    size_t response_length;

    index = 0;
    write(1, buffer, buffer_length);
    write(1, "\n", 1);
    printf("sending %zu bytes\n", buffer_length);
    while (index < buffer_length)
    {
        copied = buffer_length - index < PAYLOAD_MAX_SIZE ? buffer_length - index : PAYLOAD_MAX_SIZE;
        printf("size copied: %zu\n", copied);
        ft_memcpy(envelope.payload, buffer + index, copied);
        envelope.payload_size = copied;
        envelope.pending_size = buffer_length - (index + copied); // should be zero for the last one
        envelope.status = 20;
        response_length = get_envelope_size(copied);
        // printf("payload length: %zu\n", envelope.payload_size);
        if (send(fd, &envelope, response_length, 0) == -1)
        {
            // printf("Error cantsend to the socket");
            return (0);
        }
        index += copied;
        // printf("index: %zu, buffer_length: %zu\n", index, buffer_length);
    }
    // printf("returned after sending");
    return (1);
}