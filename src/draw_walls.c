#include "../includes/cube3d.h"

static t_texture *get_wall_texture(t_game *game, t_ray *ray)
{
    if (ray->side == 0)
    {
        if (ray->dir_x > 0)
            return (&game->textures[2]); // East texture
        return (&game->textures[3]);     // West texture
    }
    if (ray->dir_y > 0)
        return (&game->textures[1]);     // South texture
    return (&game->textures[0]);         // North texture
}

static void calculate_wall_x(t_ray *ray, t_player *player)
{
    if (ray->side == 0)
        ray->wall_x = player->y + ray->perp_wall_dist * ray->dir_y;
    else
        ray->wall_x = player->x + ray->perp_wall_dist * ray->dir_x;
    ray->wall_x -= floor(ray->wall_x);
}

void draw_textured_wall(t_game *game, t_ray *ray, int x)
{
    t_texture   *tex;
    int         tex_x;
    int         line_height;
    int         draw_start;
    int         draw_end;
    int         y;
    double      step;
    double      tex_pos;

    tex = get_wall_texture(game, ray);
    calculate_wall_x(ray, &game->player);
    tex_x = (int)(ray->wall_x * tex->width);
    if ((ray->side == 0 && ray->dir_x > 0) ||
        (ray->side == 1 && ray->dir_y < 0))
        tex_x = tex->width - tex_x - 1;

    line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);
    draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;
    if (draw_start < 0)
        draw_start = 0;
    draw_end = line_height / 2 + WINDOW_HEIGHT / 2;
    if (draw_end >= WINDOW_HEIGHT)
        draw_end = WINDOW_HEIGHT - 1;

    step = 1.0 * tex->height / line_height;
    tex_pos = (draw_start - WINDOW_HEIGHT / 2 + line_height / 2) * step;

    y = draw_start;
    while (y < draw_end)
    {
        int tex_y = (int)tex_pos & (tex->height - 1);
        tex_pos += step;
        unsigned int color = get_texture_color(tex, tex_x, tex_y);
        if (ray->side == 1)
            color = (color >> 1) & 8355711; // Make sides darker
        my_mlx_pixel_put(&game->img, x, y, color);
        y++;
    }
} 