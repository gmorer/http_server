#include "iosocket.h"

char    *path_join(char *first, char *second)
{
    char    add_size;
    size_t  first_len;
    size_t  second_len;
    char    *rslt;

    if (ft_strcmp(second, "/") == 0)
        return (ft_strdup("/"));
    add_size = 1; // default just add "/" between the 2 paths
    first_len = ft_strlen(first);
    second_len = ft_strlen(second);
    add_size -= first[first_len - 1] == '/';
    add_size -= second[0] == '/';
    add_size -= second[second_len - 1] == '/';
    if ((rslt = malloc((first_len + second_len + add_size)* sizeof(char))) == NULL)
        return (NULL);
    ft_memcpy(rslt, first, first[first_len - 1] == '/' ? --first_len : first_len);
    rslt[first_len] = '/';
    ft_memcpy(rslt + first_len + 1, second + (second[0] == '/'), second_len + (second[0] == '/' ? -1 : 0)
        + (second[second_len - 1] == '/' ? -1 : 0));
    return (rslt);
}

int     remove_dots(char *path)
{
    char    **split;
    size_t  index;
    size_t  second_index;
    size_t  head;

    if (!ft_strstr(path, ".."))
        return (path);
    index = 0;
    split = ft_strsplit(path, '/');
    while (split[index])
    {
        if (ft_strcmp(split[index], "..") == 0)
        {
            free(split[index]);
            split[index] = NULL;
        }
        index++;
    }
    second_index = 0;
    head = 0;
    while (second_index < index)
    {
        if (split[second_index])
        {
            ft_memcpy(path + head, split[second_index], ft_strlen(split[second_index]));
            head += ft_strlen(split[second_index]);
        }
        second_index += 1;
    }
    path[head] = '\0';
    printf("%s\n", path);
}

int     is_valid_path(char *path)
{
    const char current_path[PATH_MAX];
    struct stat fileStat;

    if (getcwd(current_path, sizeof(current_path) == NULL)
        return (0); // invalid current path WTF;
    if (ft_strncmp(current_path, path, ft_strlen(current_path) !== 0)
        return (0); // invalid path
    if (stat(path, &fileStat) < 0)    
        return (0); // cant read the stat :(
    if ((S_ISDIR(fileStat.st_mode) == 0))
        return (0); // is not a directory
    if (!(fileStat.st_mode & S_IXUSR))
        return (0); // you ave no rigths
    return (1);
}


int go_to(char **path, char *dest)
{
    if (!path || !*path || !dest)
        return (0); // invalid argument
    if (ft_strcmp(dest, ".") == 0)
        return (1);
}