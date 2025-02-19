/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:51:54 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/19 15:35:42 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

void	init_dda(t_ray *ray)
{
	ray->delta_dist_x = fabs(1.0 / ray->dir_x); // Calculate delta distances
	ray->delta_dist_y = fabs(1.0 / ray->dir_y);
	ray->map_x = (int)ray->pos_x; // Calculate step and initial side_dist
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

static int	check_map_bounds(t_ray *ray, t_game *game)
{
	if (ray->map_x < 0 || ray->map_x >= game->map.width || ray->map_y < 0
		|| ray->map_y >= game->map.height)
	{
		ray->hit = 1;
		return (1);
	}
	return (0);
}

static void	update_ray_position(t_ray *ray)
{
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
}

static void	calculate_wall_distance(t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - ray->pos_x + (1 - ray->step_x) / 2)
			/ ray->dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - ray->pos_y + (1 - ray->step_y) / 2)
			/ ray->dir_y;
}

void	perform_dda(t_ray *ray, t_game *game)
{
	ray->hit = 0;
	while (ray->hit == 0)
	{
		if (check_map_bounds(ray, game))
			break ;
		update_ray_position(ray);
		if (ray->map_x >= 0 && ray->map_x < game->map.width && ray->map_y >= 0
			&& ray->map_y < game->map.height
			&& game->map.map[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
	calculate_wall_distance(ray);
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
	t_ray ray;
	int x;
	double camera_x;

	x = 0;
	while (x < WINDOW_WIDTH / 2) // Only cast rays for half the screen (2D view)
	{
		camera_x = 2 * x / (double)(WINDOW_WIDTH / 2) - 1;
		calculate_ray_dir(&ray, &game->player, camera_x);
		init_dda(&ray);
		perform_dda(&ray, game);
		draw_ray_2d(game, &ray);
		x++;
	}
	// Cast rays for 3D view
	x = 0;
	while (x < WINDOW_WIDTH / 2)
	{
		camera_x = 2 * x / (double)(WINDOW_WIDTH / 2) - 1;
		calculate_ray_dir(&ray, &game->player, camera_x);
		init_dda(&ray);
		perform_dda(&ray, game);
		draw_3d_view(game, &ray, x);
		x++;
	}
}
