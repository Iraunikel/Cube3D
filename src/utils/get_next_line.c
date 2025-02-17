#include "../includes/cube3d.h"

#define BUFFER_SIZE 4096

char *get_next_line(int fd)
{
    static char buf[BUFFER_SIZE];
    static size_t idx;
    static size_t bytes_read;
    ssize_t bytes;

    char *line = NULL;
    char *tmp = NULL;
    size_t size = BUFFER_SIZE + 1;
    size_t i;
    size_t j;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return NULL;
    line = malloc(size);
    if(!line)
        return NULL;

    i = 0;
    while(1)
    {
        if(idx >= bytes_read)
        {
            bytes_read = read(fd, buf, BUFFER_SIZE);
            if(bytes_read < 0)
                return NULL;
            bytes = (size_t)bytes_read;
            idx = 0;
            if(bytes == 0)
                break;
        }

        while(i < size - 1)
        {
            line[i++] = buf[idx++];
            if(line[i - 1] == '\n')
                break;
        }

        if(i >= size - 1)
        {
            if(i < SIZE_MAX / 2)
				size *= 2;
			else
				size = SIZE_MAX;
            
            tmp = malloc(size);
            if(!tmp)
            {
                free(line);
                return NULL;
            }

            j = 0;
            while(j < i)
            {
                tmp[j] = line[j];
                j++;
            }
            free(line);
            line = tmp;
        }
        if(line[i - 1] == '\n')
            break;
    }

    if(i == 0)
    {
        free(line);
        return NULL;
    }
    
    line[i] = '\0';
    return(line);
}
