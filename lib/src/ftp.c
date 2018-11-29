#include "libftp.h"

int send_finish(int sock_fd)
{
    t_envelope envelope;

    envelope.status = 20;
    envelope.payload_size = 0;
    envelope.pending_size = 0;
    return send(sock_fd, &envelope, get_envelope_size(0), 0);
}

int send_file(int sock_fd, char *file_path, int is_client)
{
    struct stat file_stat;
    size_t      file_size;
    size_t      file_size_bckp;
    int         file_fd;
    int      read_ret;
    t_envelope  envelope;

    if (stat(file_path, &file_stat) == -1)
        return (0); 
    file_size = file_stat.st_size;
    file_size_bckp = file_size;
    if ((file_fd = open(file_path, O_RDONLY)) == -1)
        return (0);
    read_ret = 0;
    recv(sock_fd, &envelope, sizeof(t_envelope), 0); // waiting ready signal?
    ft_memset(&envelope, 0, sizeof(t_envelope));
    while ((read_ret = read(file_fd, envelope.payload, PAYLOAD_MAX_SIZE)) > 0)
    {
        //printf("%d bytes readded\n", read_ret);
        if (is_client)
        {
            printf("\33[2K\r");
            printf("%.2f%%", (float)(file_size_bckp - file_size) * 100 / (float)file_size_bckp);
            fflush(stdout);
        }
        file_size -= read_ret;
        // printf("size left: %zu\n", file_size);
        if (!send_multiple_response(sock_fd, envelope, read_ret, file_size))
        {
            close(file_fd);
            write(1, "Error sending x4300\n", 20);
            return (0); // error while sending what to do?
        }
    }
    if (is_client)
    {
        write(1, "\33[2K\r100%\n", 10);
        write(1, "finish!\n", 8);
    }
    return send_finish(sock_fd) == -1 ? 0 : 1;
}

int receive_one_chunk(int sock_fd, t_envelope *envelope)
{
    t_envelope  res_envelope;

    if (recv(sock_fd, envelope, sizeof(t_envelope), 0) == -1)
    {
        write(1, "recv error\n", 10);
        envelope->status = 40;
        return (40);
    }
    ft_memset(&res_envelope, 0, sizeof(t_envelope));
    res_envelope.status = 20;
    res_envelope.payload_size = 0;
    res_envelope.pending_size = 0;
    if (envelope->status != 21)
        return envelope->status;
    if (send(sock_fd, &res_envelope, get_envelope_size(0), 0) == -1)
    {
        envelope->status = 40;
        return 0;
    }
    return (envelope->status);
}

int receive_file(int sock_fd, char *file_path, int is_client)
{
    int file_fd;
    char status;
    char buff[sizeof(t_envelope)];
    t_envelope *envelope;
    size_t file_size;

    file_size = 0;
    ft_memset(buff, 0, sizeof(t_envelope));
    envelope = (t_envelope*)buff;
    envelope->status = 20;
    envelope->payload_size = 9;
    envelope->pending_size = 0;
    ft_memcpy(&(envelope->payload), "READYYYY\0", 9);
    if ((file_fd = open(file_path, O_WRONLY | O_APPEND | O_CREAT, 0644)) == -1)
    {
        printf("cannot open the file\n");
        return (0); // cant open the file;
    }
    write(1, "sending ready signal...\n", 25);
    if (send(sock_fd, envelope, get_envelope_size(0), 0) == -1)
    {
        write(1, "cant send ready signal\n", 23);
        return (0);
    }
    while ((status = receive_one_chunk(sock_fd, envelope)) == 21)
    {
        if (is_client)
        {
            file_size = file_size ? file_size : envelope->pending_size + envelope->payload_size;
            printf("\33[2K\r");
            printf("%.2f%%", (float)(file_size - envelope->pending_size) * 100 / (float)file_size);
            fflush(stdout);
        }
        // write(1, "1", 1);
        // printf("envelope status:%d\n", envelope->status);
        write(file_fd, envelope->payload, envelope->payload_size);
        // write(1, envelope.payload, envelope.payload_size);
    }
    close(file_fd);
    if (is_client)
    {
        write(1, "\33[2K\r100%\n", 10);
        write(1, "finish!\n", 8);
    }
    if (envelope->status != 20)
    {
        printf("payload: %s", envelope->payload);
        return (0); // something wrong happen server side :(
    }
    return (1);
}