/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:44:11 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/19 23:56:36 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static void	my_mlx_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
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

static void	calculate_line_dimensions(t_ray *ray, int *line_height,
		int *draw_start, int *draw_end)
{
	if (ray->perp_wall_dist < 0.1)
		*line_height = WINDOW_HEIGHT;
	else
		*line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);
	*draw_start = (WINDOW_HEIGHT - *line_height) / 2;
	if (*draw_start < 0)
		*draw_start = 0;
	*draw_end = (WINDOW_HEIGHT + *line_height) / 2;
	if (*draw_end >= WINDOW_HEIGHT)
		*draw_end = WINDOW_HEIGHT - 1;
}

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

void	draw_textured_wall(t_game *game, t_ray *ray, int x)
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

	calculate_line_dimensions(ray, &line_height, &draw_start, &draw_end);
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
		my_mlx_pixel_put(game, x, y, color);
		y++;
	}
}

void	draw_3d_view(t_game *game, t_ray *ray, int x)
{
	int screen_x;

	screen_x = x + WINDOW_WIDTH / 2;
	draw_textured_wall(game, ray, screen_x);
}

static void	draw_ceiling_floor(t_game *game)
{
	int	ceiling_color;
	int	floor_color;
	int	y;
	int	x;

	ceiling_color = (game->ceiling_color.r << 16) | (game->ceiling_color.g << 8) | game->ceiling_color.b;
	floor_color = (game->floor_color.r << 16) | (game->floor_color.g << 8) | game->floor_color.b;
	
	printf("Debug - Colors:\n");
	printf("Ceiling RGB: %d,%d,%d (hex: 0x%06X)\n", 
		game->ceiling_color.r, game->ceiling_color.g, game->ceiling_color.b, ceiling_color);
	printf("Floor RGB: %d,%d,%d (hex: 0x%06X)\n", 
		game->floor_color.r, game->floor_color.g, game->floor_color.b, floor_color);

	// Draw ceiling (top half)
	y = 0;
	while (y < WINDOW_HEIGHT / 2)
	{
		x = WINDOW_WIDTH / 2;
		while (x < WINDOW_WIDTH)
		{
			my_mlx_pixel_put(game, x, y, ceiling_color);
			x++;
		}
		y++;
	}
	
	// Draw floor (bottom half)
	y = WINDOW_HEIGHT / 2;
	while (y < WINDOW_HEIGHT)
	{
		x = WINDOW_WIDTH / 2;
		while (x < WINDOW_WIDTH)
		{
			my_mlx_pixel_put(game, x, y, floor_color);
			x++;
		}
		y++;
	}
}

int	game_loop(t_game *game)
{
	static int		frame_count = 0;
	static clock_t	last_time = 0;
	clock_t			current_time;

	// Clear the entire screen
	ft_bzero(game->addr, WINDOW_WIDTH * WINDOW_HEIGHT * (game->bits_per_pixel / 8));
	
	// Update player position
	move_player(game);
	
	// Draw 3D view background first
	draw_ceiling_floor(game);
	
	// Draw 2D map and cast rays
	draw_map(game);
	cast_rays(game);
	
	// FPS calculation (debug)
	frame_count++;
	current_time = clock();
	if (current_time - last_time >= CLOCKS_PER_SEC)
	{
		printf("FPS: %d\n", frame_count);
		frame_count = 0;
		last_time = current_time;
	}
	
	// Update screen
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}
