#include "../includes/cube3d.h"
#include <../minilibx_opengl_20191021/mlx.h>

static int init_mlx(t_game *game)
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
    return (1);
}

static int init_image(t_game *game)
{
    game->img = mlx_new_image(game->mlx, 1000, 1000);
    if (!game->img)
    {
        mlx_destroy_window(game->mlx, game->win);
        free(game->mlx);
        return (0);
    }
    game->addr = mlx_get_data_addr(game->img, &game->bits_per_pixel,
                                  &game->line_length, &game->endian);
    return (1);
}

static void init_map_and_player(t_game *game)
{
    game->player.x = 0;
    game->player.y = 0;
    game->map.map = NULL;
    game->map.width = 0;
    game->map.height = 0;
}

static void init_colors_and_textures(t_game *game)
{
    game->ceiling_color = (t_color){0, 0, 0};
    game->floor_color = (t_color){128, 128, 128};
    game->north_texture = NULL;
    game->south_texture = NULL;
    game->west_texture = NULL;
    game->east_texture = NULL;
}

int init_game(t_game *game)
{
    if (!init_mlx(game))
        return (0);
    if (!init_image(game))
        return (0);
    init_map_and_player(game);
    init_colors_and_textures(game);
    return (1);
}
