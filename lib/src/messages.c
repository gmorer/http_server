#include "libftp.h"

void socket_error_msg()
{
    write(2, "Socket error.\n", 14);
}

int send_success(int fd, char *message, size_t length)
{
    t_envelope envelope;

    if (!message)
    {
        message = "Operation success.";
        length = 18;
    }
    envelope.status = 20;
    ft_strncpy(envelope.payload, message, PAYLOAD_MAX_SIZE);
    envelope.pending_size = 0;
    envelope.payload_size = length;
    if (send(fd, &envelope, get_envelope_size(length), 0) == -1)
    {
        socket_error_msg();
        return (0);
    }
    return (1);
}

int send_failure(int fd, char *message, size_t length)
{
    t_envelope envelope;

    if (!message)
    {
        message = "Operation failure.";
        length = 18;
    }
    if (!length)
        length = ft_strlen(message);
    envelope.status = 44;
    ft_strncpy(envelope.payload, message, PAYLOAD_MAX_SIZE);
    envelope.pending_size = 0;
    envelope.payload_size = length;
    if (send(fd, &envelope, get_envelope_size(length), 0) == -1)
    {
        socket_error_msg();
        return (0);
    }
    return (0);
}