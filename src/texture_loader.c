#include "../includes/cube3d.h"

static void init_texture(t_texture *tex, void *mlx, char *path)
{
    printf("Loading texture from path: %s\n", path);
    tex->img = mlx_xpm_file_to_image(mlx, path, &tex->width, &tex->height);
    if (!tex->img)
        error_exit("Failed to load texture");
    tex->addr = mlx_get_data_addr(tex->img, &tex->bits_per_pixel,
        &tex->line_length, &tex->endian);
    if (!tex->addr)
        error_exit("Failed to get texture address");
}

void    load_textures(t_game *game)
{
    printf("Debug - Texture paths:\n");
    printf("North: %s\n", game->north_texture);
    printf("South: %s\n", game->south_texture);
    printf("West: %s\n", game->west_texture);
    printf("East: %s\n", game->east_texture);
    printf("Floor: %s (Use texture: %d)\n", game->floor_texture, game->use_texture_floor);
    printf("Ceiling: %s (Use texture: %d)\n", game->ceiling_texture, game->use_texture_ceiling);

    if (!game->north_texture || !game->south_texture || 
        !game->west_texture || !game->east_texture)
        error_exit("Missing wall texture path");

    // Load wall textures
    init_texture(&game->textures[TEX_NORTH], game->mlx, game->north_texture);
    init_texture(&game->textures[TEX_SOUTH], game->mlx, game->south_texture);
    init_texture(&game->textures[TEX_EAST], game->mlx, game->east_texture);
    init_texture(&game->textures[TEX_WEST], game->mlx, game->west_texture);

    // Load floor and ceiling textures if specified
    if (game->use_texture_floor)
    {
        if (!game->floor_texture)
            error_exit("Floor texture enabled but no path specified");
        init_texture(&game->textures[TEX_FLOOR], game->mlx, game->floor_texture);
    }
    
    if (game->use_texture_ceiling)
    {
        if (!game->ceiling_texture)
            error_exit("Ceiling texture enabled but no path specified");
        init_texture(&game->textures[TEX_CEIL], game->mlx, game->ceiling_texture);
    }
}

unsigned int get_texture_color(t_texture *tex, int x, int y)
{
    char    *pixel;
    int     offset;

    offset = (y * tex->line_length + x * (tex->bits_per_pixel / 8));
    pixel = tex->addr + offset;
    return (*(unsigned int *)pixel);
} 