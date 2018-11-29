#include "ftp_client.h"

int fn_put(int sock, char **argv)
{
    t_envelope envelope;

    if (!argv[1])
    {
        write(2, "Bad usage, usage: put [FILE].\n", 30);
        return (0);
    }
    ft_memset(&envelope, 0, sizeof(t_envelope));
    envelope.status = 41;
    envelope.pending_size = 0;
    envelope.payload_size = ft_strlen(argv[1]) + 1;
    ft_memcpy(&(envelope.payload), argv[1], ft_strlen(argv[1]));
    printf("filename:::: %s\n", envelope.payload);
    fflush(stdout);
    write(1, "Starting\n", 9);
    send(sock, &envelope, get_envelope_size(ft_strlen(argv[1]) + 1), 0);
    write(1, "filename sended\n", 16);
    send_file(sock, argv[1], 0);
    write(1, "put\n", 4);
    return (1);
}