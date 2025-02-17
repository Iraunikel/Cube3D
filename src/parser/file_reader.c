#include "../includes/cube3d.h"

int validate_file_extension(const char *filename)
{
    size_t  len;
    char    *extension;

    if (!filename)
        return (0);
    
    len = ft_strlen(filename);
    if (len < 5)  // Need at least "x.cub" (1 char + 4 chars extension)
        return (0);
    
    extension = ft_strrchr(filename, '.');
    if (!extension)
        return (0);
    
    if (ft_strncmp(extension, ".cub", 4) != 0)
        return (0);
    
    return (1);
}
