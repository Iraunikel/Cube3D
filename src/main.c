#include "../includes/cube3d.h"

int main(int argc, char **argv)
{
    t_game  game;

    if (argc != 2)
    {
        ft_putstr_fd("Error\nUsage: ./cub3D <map.cub>\n", 2);
        return (1);
    }
    if (!validate_file_extension(argv[1]))
    {
        ft_putstr_fd("Error\nInvalid file extension. Must be .cub\n", 2);
        return (1);
    }
    if (!init_game(&game))
    {
        ft_putstr_fd("Error\nFailed to initialize game\n", 2);
        return (1);
    }
    if (!parse_map_file(argv[1], &game))
    {
        cleanup_game(&game);
        return (1);
    }
    //game_loop(&game); we havent developed the game loop yet
    cleanup_game(&game);
    return (0);
}

