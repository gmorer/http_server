#include "ftp_client.h"

int fn_get(int sock, char **argv)
{
    t_envelope envelope;

    if (!argv[1])
    {
        write(2, "Bad usage, usage: put [FILE].\n", 30);
        return (0);
    }
    ft_memset(&envelope, 0, sizeof(t_envelope));
    envelope.status = 31;
    envelope.pending_size = 0;
    envelope.payload_size = ft_strlen(argv[1]) + 1;
    ft_memcpy(&(envelope.payload), argv[1], ft_strlen(argv[1]));
    send(sock, &envelope, get_envelope_size(ft_strlen(argv[1]) + 1), 0);
    receive_file(sock, argv[1], 1);
    return (1);
}