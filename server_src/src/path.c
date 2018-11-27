#include "iosocket.h"

char g_root_path[PATH_MAX];

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
    if ((rslt = malloc((first_len + second_len + add_size + 1) * sizeof(char))) == NULL)
        return (NULL);
    ft_memset(rslt, 0, (first_len + second_len + add_size)* sizeof(char));
    ft_memcpy(rslt, first, first[first_len - 1] == '/' ? --first_len : first_len);
    rslt[first_len] = '/';
    ft_memcpy(rslt + first_len + 1, second + (second[0] == '/'), second_len + (second[0] == '/' ? -1 : 0)
        + (second[second_len - 1] == '/' ? -1 : 0));
    printf("new_path: %s\n", rslt);
    return (rslt);
}

int     remove_dots(char *path)
{
    char    **split;
    size_t  index;
    size_t  second_index;
    size_t  head;

    if (!ft_strstr(path, ".."))
        return (1);
    index = 0;
    split = ft_strsplit(path, '/');
    while (split[index])
    {
        if (ft_strcmp(split[index], "..") == 0)
        {
            free(split[index]);
            split[index] = NULL;
            second_index = index;
            while (second_index && !split[second_index])
                second_index -= 1;
            if (split[second_index])
            {
                free(split[second_index]);
                split[second_index] = NULL;
            }
        }
        index++;
    }
    second_index = 0;
    head = 0;
    while (second_index < index)
    {
        if (split[second_index])
        {
            path[head++] = '/';
            ft_memcpy(path + head, split[second_index], ft_strlen(split[second_index]));
            head += ft_strlen(split[second_index]);
        }
        second_index += 1;
    }
    ft_strtsrfree(split);
    path[head] = '\0';
    printf("\npath: %s\n", path);
    return (1);
}

int     is_valid_path(char *path)
{
    struct stat file_stat;

    if (ft_strncmp(g_root_path, path, ft_strlen(g_root_path)) != 0)
        return (0); // invalid path
    if (stat(path, &file_stat) < 0)    
        return (0); // cant read the stat :(
    if ((S_ISDIR(file_stat.st_mode) == 0))
        return (0); // is not a directory
    if (!(file_stat.st_mode & S_IXUSR))
        return (0); // you ave no rigths
    printf("path is valid:, root_path: %s, asked path: %s\n", g_root_path, path);
    return (1);
}


int go_to(char **path, char *dest)
{
    char *new_path;

    if (!path || !*path || !dest)
    {
        printf("path?: %d\n", !!path);
        return (0);
        printf("Inalid argument: path: %d, *path: %s, dest: %s\n", !!path, *path, dest);
        return (0); // invalid argument
    }
    if (ft_strcmp(dest, ".") == 0)
        return (1);
    printf("old pah: %s\n", *path);
    new_path = path_join(*path, dest);
    remove_dots(new_path);
    if (is_valid_path(new_path))
    {
        *path = new_path;
        return (1);
    }
    return (0);
}