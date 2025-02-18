/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:44:11 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/18 15:52:27 by iunikel          ###   ########.fr       */
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

	size = 50;
	start_x = x * size + (1000 - (game->map.width * size)) / 2;
	start_y = y * size + (1000 - (game->map.height * size)) / 2;
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

static void	draw_player(t_game *game)
{
	int		size;
	int		px;
	int		py;
	int		line_length;
	double	dir_x;
	double	dir_y;
	int		i;
	int		j;

	size = 50;
	px = (int)(game->player.x * size + (1000 - (game->map.width * size)) / 2);
	py = (int)(game->player.y * size + (1000 - (game->map.height * size)) / 2);
	// Draw player position (smaller square)
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
	// Draw direction line
	line_length = 30;
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
	double  delta_x;
	double  delta_y;
	double  step;
	double  x;
	double  y;
	int     i;

	delta_x = x2 - x1;
	delta_y = y2 - y1;
	step = fabs(delta_x) > fabs(delta_y) ? fabs(delta_x) : fabs(delta_y);
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

void	draw_ray_2d(t_game *game, t_ray *ray)
{
	int size;
	int start_x;
	int start_y;
	int end_x;
	int end_y;

	size = 50;  // Same as in draw_square
	
	// Convert world coordinates to screen coordinates
	start_x = ray->pos_x * size + (WINDOW_WIDTH - (game->map.width * size)) / 2;
	start_y = ray->pos_y * size + (WINDOW_HEIGHT - (game->map.height * size)) / 2;
	end_x = start_x + (ray->dir_x * size);
	end_y = start_y + (ray->dir_y * size);
	
	draw_line(game, start_x, start_y, end_x, end_y, 0x0000FF);  // Blue for rays
}

int	game_loop(t_game *game)
{
	// Update player position based on movement flags
	move_player(game);

	// Draw 2D map
	draw_map(game);

	// Cast rays
	cast_rays(game);

	// Update screen
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);

	return (0);
}