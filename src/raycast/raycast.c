/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:51:54 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/18 15:51:56 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

void	calculate_ray_dir(t_ray *ray, t_player *player, double camera_x)
{
    // Calculate ray direction
    ray->dir_x = player->dir_x + player->plane_x * camera_x;
    ray->dir_y = player->dir_y + player->plane_y * camera_x;
    
    // Set ray starting position
    ray->pos_x = player->x;
    ray->pos_y = player->y;
}

void	cast_rays(t_game *game)
{
    t_ray   ray;
    int     x;
    double  camera_x;

    x = 0;
    while (x < WINDOW_WIDTH)
    {
        // Calculate camera x-coordinate in camera space
        camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
        
        // Calculate ray direction
        calculate_ray_dir(&ray, &game->player, camera_x);
        
        // For now, just draw a debug line in the 2D view
        draw_ray_2d(game, &ray);
        x++;
    }
} 