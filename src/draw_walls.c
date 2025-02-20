/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_walls.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 12:24:30 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/20 20:49:27 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    int         tex_y;
    int         line_height;
    int         draw_start;
    int         draw_end;
    int         y;
    double      step;
    double      tex_pos;

    // Get the appropriate texture based on which wall was hit
    tex = get_wall_texture(game, ray);

    // Calculate where exactly the wall was hit
    calculate_wall_x(ray, &game->player);

    // Calculate x coordinate on the texture
    tex_x = (int)(ray->wall_x * TEX_WIDTH);
    if ((ray->side == 0 && ray->dir_x > 0) ||
        (ray->side == 1 && ray->dir_y < 0))
        tex_x = TEX_WIDTH - tex_x - 1;

    // Calculate the height of the line to draw
    line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);

    // Calculate lowest and highest pixel to fill in current stripe
    draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;
    if (draw_start < 0)
        draw_start = 0;
    draw_end = line_height / 2 + WINDOW_HEIGHT / 2;
    if (draw_end >= WINDOW_HEIGHT)
        draw_end = WINDOW_HEIGHT - 1;

    // Calculate step for texture coordinate
    step = 1.0 * TEX_HEIGHT / line_height;
    // Starting texture coordinate
    tex_pos = (draw_start - WINDOW_HEIGHT / 2 + line_height / 2) * step;

    // Draw the vertical stripe
    y = draw_start;
    while (y < draw_end)
    {
        // Cast tex_pos to integer and ensure it stays within bounds
        tex_y = (int)tex_pos & (TEX_HEIGHT - 1);
        tex_pos += step;

        // Get color from texture
        unsigned int color = get_texture_color(tex, tex_x, tex_y);

        // Make sides darker for depth effect
        if (ray->side == 1)
            color = (color >> 1) & 8355711;

        // Draw the pixel
        my_mlx_pixel_put(game, x, y, color);
        y++;
    }
} 