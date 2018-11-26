#include "libftp.h"

char    get_status(char *buffer)
{
    static size_t offset = ((size_t)&((t_envelope*)0)->status);
    return *(buffer + offset);
}

size_t get_pending_size(char *buffer)
{
    static size_t offset = ((size_t)&((t_envelope*)0)->pending_size);
    return *((size_t*)(buffer + offset));
}

size_t get_payload_size(char *buffer)
{
    static size_t offset = ((size_t)&((t_envelope*)0)->payload_size);
    return *((size_t*)(buffer + offset));
}

void get_payload(char *buffer, t_envelope *envelope, size_t payload_size)
{
    static size_t offset = ((size_t)&((t_envelope*)0)->payload);
    ft_memcpy((*envelope).payload, buffer + offset, payload_size);
}

void    fill_envelope(char *buffer, t_envelope *envelope)
{
    ft_memset(envelope, 0, sizeof(envelope));
    (*envelope).status = get_status(buffer);
    (*envelope).pending_size = get_pending_size(buffer);
    if (((*envelope).payload_size = get_payload_size(buffer)))
    {
        get_payload(buffer, envelope, (*envelope).payload_size);
    }
}
