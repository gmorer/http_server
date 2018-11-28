#include "libftp.h"

int send_finish(int sock_fd)
{
    t_envelope envelope;

    envelope.status = 20;
    envelope.payload_size = 0;
    envelope.pending_size = 0;
    return send(sock_fd, &envelope, sizeof(char), 0);
}

int send_file(int sock_fd, char *file_path)
{
    struct stat file_stat;
    size_t      file_size;
    int         file_fd;
    int      read_ret;
    char        buff[PAYLOAD_MAX_SIZE];

  // stat() returns -1 on error. Skipping check in this example
    if (stat(file_path, &file_stat) == -1)
        return (0); 
    file_size = file_stat.st_size;
    if ((file_fd = open(file_path, O_RDONLY)) == -1)
        return (0);
    read_ret = 0;
    recv(sock_fd, buff, sizeof(PAYLOAD_MAX_SIZE), 0);
    while ((read_ret = read(file_fd, buff, PAYLOAD_MAX_SIZE)) > 0)
    {
        printf("%d bytes readded\n", read_ret);
        fflush(stdout);
        if (read_ret == -1)
        {
            // wtf do something
            // maybe happen because of the deletion of the file durring the read or something like that
            close(file_fd);
            send_failure(sock_fd, "Error while reading the file.", 0);
        }
        file_size -= read_ret;
        printf("size left: %zu\n", file_size);
        if (!send_multiple_response(sock_fd, buff, read_ret, file_size))
        {
            close(file_fd);
            write(1, "Error sending x4300\n", 20);
            return (0); // error while sending what to do?
        }
    }
    write(1, "222222\n", 7);
    write(1, "finish!\0", 8);
    return send_finish(sock_fd);
}

t_envelope receive_one_chunk(int sock_fd, t_envelope *envelope)
{
    char        buff[sizeof(t_envelope)];
    t_envelope  res_envelope;

    res_envelope.status = 20;
    res_envelope.payload_size = 0;
    res_envelope.pending_size = 0;
    recv(sock_fd, buff, sizeof(t_envelope), 0);
    fill_envelope(buff, envelope);
    send(sock_fd, &res_envelope, get_envelope_size(0), 0);
    return (*envelope);
}

int receive_file(int sock_fd, char *file_path)
{
    int file_fd;
    char status;
    t_envelope envelope;

    envelope.status = 20;
    envelope.payload_size = 9;
    envelope.pending_size = 0;
    ft_memcpy(&(envelope.payload), "READYYYY\0", 9);
    if ((file_fd = open(file_path, O_WRONLY | O_APPEND | O_CREAT, 0644)) == -1)
    {
        printf("cannot open the file\n");
        return (0); // cant open the file;
    }
    write(1, "sending ready signal...", 24);
    send(sock_fd, &envelope, get_envelope_size(0), 0);
    while ((status = receive_one_chunk(sock_fd, &envelope).status) == 21)
    {
        write(1, "1", 1);
        write(file_fd, envelope.payload, envelope.payload_size);
        // write(1, envelope.payload, envelope.payload_size);
    }
    write(1, "FINISH\n", 7);
    if (envelope.status != 20)
    {
        printf("payload: %s", envelope.payload);
        return (0); // something wrong happen server side :(
    }
    return (1);
}