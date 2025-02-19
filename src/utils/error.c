#include "../../includes/cube3d.h"

void    error_exit(const char *message)
{
    ft_putstr_fd("Error\n", 2);
    ft_putstr_fd(message, 2);
    ft_putstr_fd("\n", 2);
    exit(1);
} 