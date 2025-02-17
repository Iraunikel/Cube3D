#include "../includes/cube3d.h"
#include <../minilibx_opengl_20191021/mlx.h>

int init_game(t_game *game)
{
    game->mlx = mlx_init();
    if (!game->mlx)
        return (0);
    game->win = mlx_new_window(game->mlx, 1000, 1000, "Cube3D");
    if (!game->win)
    {
        free(game->mlx);
        return (0);
    }
    
    // Create image buffer
    game->img = mlx_new_image(game->mlx, 1000, 1000);
    if (!game->img)
    {
        mlx_destroy_window(game->mlx, game->win);
        free(game->mlx);
        return (0);
    }
    
    // Get image data
    game->addr = mlx_get_data_addr(game->img, &game->bits_per_pixel,
                                  &game->line_length, &game->endian);
    
    // Initialize player
    game->player.x = 0;
    game->player.y = 0;
    
    // Initialize map
    game->map.map = NULL;
    game->map.width = 0;
    game->map.height = 0;
    
    // Initialize colors (ceiling and floor)
    game->ceiling_color = (t_color){0, 0, 0};  // Black by default
    game->floor_color = (t_color){128, 128, 128};  // Gray by default
    
    // Initialize texture pointers to NULL
    game->north_texture = NULL;
    game->south_texture = NULL;
    game->west_texture = NULL;
    game->east_texture = NULL;
    
    return (1);
}