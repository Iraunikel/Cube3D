/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:51:54 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/19 13:11:56 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static void	init_dda(t_ray *ray)
{
    // Calculate delta distances
    ray->delta_dist_x = fabs(1.0 / ray->dir_x);
    ray->delta_dist_y = fabs(1.0 / ray->dir_y);
    
    // Calculate step and initial side_dist
    ray->map_x = (int)ray->pos_x;
    ray->map_y = (int)ray->pos_y;
    
    if (ray->dir_x < 0)
    {
        ray->step_x = -1;
        ray->side_dist_x = (ray->pos_x - ray->map_x) * ray->delta_dist_x;
    }
    else
    {
        ray->step_x = 1;
        ray->side_dist_x = (ray->map_x + 1.0 - ray->pos_x) * ray->delta_dist_x;
    }
    
    if (ray->dir_y < 0)
    {
        ray->step_y = -1;
        ray->side_dist_y = (ray->pos_y - ray->map_y) * ray->delta_dist_y;
    }
    else
    {
        ray->step_y = 1;
        ray->side_dist_y = (ray->map_y + 1.0 - ray->pos_y) * ray->delta_dist_y;
    }
}

static void	perform_dda(t_ray *ray, t_game *game)
{
    ray->hit = 0;
    
    while (ray->hit == 0)
    {
        // Check if we're still within map bounds
        if (ray->map_x < 0 || ray->map_x >= game->map.width ||
            ray->map_y < 0 || ray->map_y >= game->map.height)
        {
            ray->hit = 1;
            break;
        }

        if (ray->side_dist_x < ray->side_dist_y)
        {
            ray->side_dist_x += ray->delta_dist_x;
            ray->map_x += ray->step_x;
            ray->side = 0;
        }
        else
        {
            ray->side_dist_y += ray->delta_dist_y;
            ray->map_y += ray->step_y;
            ray->side = 1;
        }
        
        // Check if we hit a wall
        if (ray->map_x >= 0 && ray->map_x < game->map.width &&
            ray->map_y >= 0 && ray->map_y < game->map.height &&
            game->map.map[ray->map_y][ray->map_x] == '1')
        {
            ray->hit = 1;
        }
    }
    
    // Calculate perpendicular wall distance
    if (ray->side == 0)
        ray->perp_wall_dist = (ray->map_x - ray->pos_x + 
            (1 - ray->step_x) / 2) / ray->dir_x;
    else
        ray->perp_wall_dist = (ray->map_y - ray->pos_y + 
            (1 - ray->step_y) / 2) / ray->dir_y;
}

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
        
        // Initialize DDA
        init_dda(&ray);
        
        // Perform DDA
        perform_dda(&ray, game);
        
        // For now, just draw a debug line in the 2D view
        draw_ray_2d(game, &ray);
        x++;
    }
} 