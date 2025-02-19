/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:44:11 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/19 15:34:14 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static void	my_mlx_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= 1000 || y < 0 || y >= 1000)
		return ;
	dst = game->addr + (y * game->line_length + x * (game->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

static void	draw_square(t_game *game, int x, int y, int color)
{
	int	i;
	int	j;
	int	size;
	int	start_x;
	int	start_y;

	size = 25;
	// Reduced size for the 2D view
	start_x = x * size + (WINDOW_WIDTH / 4 - (game->map.width * size) / 2);
	// Center in left half
	start_y = y * size + (WINDOW_HEIGHT / 2 - (game->map.height * size) / 2);
	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1)
		{
			my_mlx_pixel_put(game, start_x + i, start_y + j, color);
			j++;
		}
		i++;
	}
}

static void	draw_player_position(t_game *game, int px, int py)
{
	int	i;
	int	j;

	i = -2;
	while (i < 3)
	{
		j = -2;
		while (j < 3)
		{
			my_mlx_pixel_put(game, px + i, py + j, 0xFF0000);
			j++;
		}
		i++;
	}
}

static void	draw_player_direction(t_game *game, int px, int py)
{
	int		line_length;
	double	dir_x;
	double	dir_y;
	int		i;

	line_length = 15;
	dir_x = game->player.dir_x * line_length;
	dir_y = game->player.dir_y * line_length;
	i = 0;
	while (i < line_length)
	{
		my_mlx_pixel_put(game, px + (int)(dir_x * i / line_length), py
			+ (int)(dir_y * i / line_length), 0x00FF00);
		i++;
	}
}

static void	draw_player(t_game *game)
{
	int	size;
	int	px;
	int	py;

	size = 25;
	px = (int)(game->player.x * size + (WINDOW_WIDTH / 4 - (game->map.width
					* size) / 2));
	py = (int)(game->player.y * size + (WINDOW_HEIGHT / 2 - (game->map.height
					* size) / 2));
	draw_player_position(game, px, py);
	draw_player_direction(game, px, py);
}

static void	draw_map(t_game *game)
{
	int	x;
	int	y;

	// Clear screen to dark gray
	ft_bzero(game->addr, 1000 * 1000 * (game->bits_per_pixel / 8));
	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (game->map.map[y][x] == '1')
				draw_square(game, x, y, 0xFFFFFF); // White walls
			else if (game->map.map[y][x] == '0' || ft_strchr("NSEW",
					game->map.map[y][x]))
				draw_square(game, x, y, 0x333333); // Dark gray floor
			x++;
		}
		y++;
	}
	draw_player(game);
}

static void	draw_line(t_game *game, int x1, int y1, int x2, int y2, int color)
{
	double	delta_x;
	double	delta_y;
	double	step;
	double	x;
	double	y;
	int		i;

	delta_x = x2 - x1;
	delta_y = y2 - y1;
	if (fabs(delta_x) > fabs(delta_y))
		step = fabs(delta_x);
	else
		step = fabs(delta_y);
	delta_x /= step;
	delta_y /= step;
	x = x1;
	y = y1;
	i = 0;
	while (i <= step)
	{
		my_mlx_pixel_put(game, (int)x, (int)y, color);
		x += delta_x;
		y += delta_y;
		i++;
	}
}

static void	calculate_hit_point(t_ray *ray, double *hit_x, double *hit_y)
{
	*hit_x = ray->pos_x + ray->perp_wall_dist * ray->dir_x;
	*hit_y = ray->pos_y + ray->perp_wall_dist * ray->dir_y;
}

static void	calculate_screen_coords(t_game *game, t_ray *ray, int size,
		int coords[4])
{
	double	hit_x;
	double	hit_y;

	calculate_hit_point(ray, &hit_x, &hit_y);
	coords[0] = ray->pos_x * size + (WINDOW_WIDTH / 4 - (game->map.width * size)
			/ 2);
	coords[1] = ray->pos_y * size + (WINDOW_HEIGHT / 2 - (game->map.height
				* size) / 2);
	coords[2] = hit_x * size + (WINDOW_WIDTH / 4 - (game->map.width * size)
			/ 2);
	coords[3] = hit_y * size + (WINDOW_HEIGHT / 2 - (game->map.height * size)
			/ 2);
}

void	draw_ray_2d(t_game *game, t_ray *ray)
{
	int	size;
	int	coords[4];

	size = 25;
	calculate_screen_coords(game, ray, size, coords);
	if (coords[0] >= 0 && coords[0] < WINDOW_WIDTH / 2 && coords[1] >= 0
		&& coords[1] < WINDOW_HEIGHT && coords[2] >= 0
		&& coords[2] < WINDOW_WIDTH / 2 && coords[3] >= 0
		&& coords[3] < WINDOW_HEIGHT)
	{
		draw_line(game, coords[0], coords[1], coords[2], coords[3], 0x0000FF);
	}
}

static void	draw_vertical_line(t_game *game, int x, int start_y, int end_y,
		int color)
{
	int	y;

	y = start_y;
	while (y <= end_y)
	{
		if (y >= 0 && y < WINDOW_HEIGHT)
			my_mlx_pixel_put(game, x + WINDOW_WIDTH / 2, y, color);
		y++;
	}
}

void	draw_3d_view(t_game *game, t_ray *ray, int x)
{
	int	line_height;
	int	draw_start;
	int	draw_end;
	int	color;

	// Calculate line height
	line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);
	// Calculate lowest and highest pixel to fill in current stripe
	draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + WINDOW_HEIGHT / 2;
	if (draw_end >= WINDOW_HEIGHT)
		draw_end = WINDOW_HEIGHT - 1;
	// Choose wall color based on side (darker for y-side)
	color = 0xFFFFFF; // White for x-side
	if (ray->side == 1)
		color = 0xCCCCCC; // Lighter gray for y-side
	// Draw the vertical line
	draw_vertical_line(game, x, draw_start, draw_end, color);
}

static void	draw_ceiling_floor(t_game *game)
{
	int	x;
	int	y;
	int	ceiling_color;
	int	floor_color;

	ceiling_color = game->ceiling_color.r << 16 | game->ceiling_color.g << 8 | game->ceiling_color.b;
	floor_color = game->floor_color.r << 16 | game->floor_color.g << 8 | game->floor_color.b;
	x = WINDOW_WIDTH / 2;
	while (x < WINDOW_WIDTH)
	{
		y = 0;
		while (y < WINDOW_HEIGHT)
		{
			if (y < WINDOW_HEIGHT / 2)
				my_mlx_pixel_put(game, x, y, ceiling_color);
			else
				my_mlx_pixel_put(game, x, y, floor_color);
			y++;
		}
		x++;
	}
}

int	game_loop(t_game *game)
{
	// Clear screen
	ft_bzero(game->addr, WINDOW_WIDTH * WINDOW_HEIGHT * (game->bits_per_pixel
			/ 8));
	// Update player position based on movement flags
	move_player(game);
	// Draw ceiling and floor for 3D view
	draw_ceiling_floor(game);
	// Draw 2D map and cast rays
	draw_map(game);
	cast_rays(game);
	// Update screen
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}
