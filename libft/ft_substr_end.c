#include <stdlib.h>
#include "libft.h"

char *ft_substr_end(char const *src, size_t start, size_t end)
{
    char    *str;
    size_t  i;
    size_t  len;

    if (!src)
        return (NULL);
    len = end - start;
    str = malloc((len + 1) * sizeof(char));
    if (!str)
        return (NULL);
    i = 0;
    while (i < len && src[start + i])
    {
        str[i] = src[start + i];
        i++;
    }
    str[i] = '\0';
    return (str);
}