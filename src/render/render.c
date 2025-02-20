/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:44:11 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/20 19:01:24 by iunikel          ###   ########.fr       */
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
	
	// Improve precision for wall_x calculation
	ray->wall_x -= floor(ray->wall_x);
	ray->wall_x = fmax(0.0, fmin(1.0, ray->wall_x));
	
	// Calculate texture coordinate with better precision
	tex_x = (int)(ray->wall_x * (double)tex->width);
	if ((ray->side == 0 && ray->dir_x > 0) ||
		(ray->side == 1 && ray->dir_y < 0))
		tex_x = tex->width - tex_x - 1;
	
	// Ensure tex_x stays within bounds with proper clamping
	tex_x = (tex_x < 0) ? 0 : tex_x;
	tex_x = (tex_x >= tex->width) ? tex->width - 1 : tex_x;

	calculate_line_dimensions(ray, &line_height, &draw_start, &draw_end);
	
	// Improve step calculation precision
	step = 1.0 * (double)tex->height / (double)line_height;
	tex_pos = (draw_start - WINDOW_HEIGHT / 2 + line_height / 2) * step;
	
	// Add tiny offset to avoid floating point precision issues
	tex_pos += 0.00001;

	y = draw_start;
	while (y < draw_end)
	{
		// Calculate tex_y with improved precision
		int tex_y = (int)tex_pos & (tex->height - 1);
		tex_y = (tex_y < 0) ? 0 : tex_y;
		tex_y = (tex_y >= tex->height) ? tex->height - 1 : tex_y;
		
		// Get base color
		unsigned int color = get_texture_color(tex, tex_x, tex_y);
		
		// Extract RGB components
		int r = (color >> 16) & 0xFF;
		int g = (color >> 8) & 0xFF;
		int b = color & 0xFF;
		
		// Calculate distance factor with non-linear falloff
		double dist_factor = 1.0;
		if (ray->perp_wall_dist > 1.0)
		{
			dist_factor = 1.0 / (1.0 + (ray->perp_wall_dist - 1.0) * 0.3);
			dist_factor = fmax(0.3, dist_factor); // Don't make it too dark
		}
		
		// Apply side shading with perspective
		if (ray->side == 1)
		{
			double side_factor = 0.7 + 0.3 * (1.0 - fabs(ray->dir_y));
			dist_factor *= side_factor;
		}
		
		// Apply height-based shading for more depth
		double height_factor = 1.0 - fabs((double)(y - WINDOW_HEIGHT / 2) / (double)WINDOW_HEIGHT) * 0.2;
		dist_factor *= height_factor;
		
		// Apply all lighting factors
		r = (int)(r * dist_factor);
		g = (int)(g * dist_factor);
		b = (int)(b * dist_factor);
		
		// Clamp RGB values
		r = (r < 0) ? 0 : (r > 255) ? 255 : r;
		g = (g < 0) ? 0 : (g > 255) ? 255 : g;
		b = (b < 0) ? 0 : (b > 255) ? 255 : b;
		
		color = (r << 16) | (g << 8) | b;
		my_mlx_pixel_put(game, x, y, color);
		tex_pos += step;
		y++;
	}
}

void	draw_3d_view(t_game *game, t_ray *ray, int x)
{
	int screen_x;

	screen_x = x + WINDOW_WIDTH / 2;
	draw_textured_wall(game, ray, screen_x);
}

static void calculate_floor_position(double *ray_dir, int x, int y, t_floor_calc *calc)
{
    // Ray direction for leftmost ray (x = 0) and rightmost ray (x = w)
    double ray_dir_x0 = ray_dir[0];
    double ray_dir_y0 = ray_dir[1];
    double ray_dir_x1 = ray_dir[2];
    double ray_dir_y1 = ray_dir[3];

    // Current y position compared to horizon
    double p = y - WINDOW_HEIGHT / 2.0;
    
    // Vertical position of the camera
    double pos_z = 0.5 * WINDOW_HEIGHT;

    // Horizontal distance from the camera to the floor for the current row
    double row_distance = pos_z / p;

    // Calculate the real world coordinates of the leftmost column
    double floor_step_x = row_distance * (ray_dir_x1 - ray_dir_x0) / WINDOW_WIDTH;
    double floor_step_y = row_distance * (ray_dir_y1 - ray_dir_y0) / WINDOW_WIDTH;

    calc->floor_x = calc->player_x + row_distance * ray_dir_x0;
    calc->floor_y = calc->player_y + row_distance * ray_dir_y0;

    // Add the step for this x coordinate
    double relative_x = x - WINDOW_WIDTH / 2.0;
    calc->floor_x += floor_step_x * relative_x;
    calc->floor_y += floor_step_y * relative_x;
}

static void get_texture_coordinates(t_texture *tex, t_floor_calc *calc, int *tex_coords)
{
    // Get the texture coordinates
    double fx = calc->floor_x - floor(calc->floor_x);
    double fy = calc->floor_y - floor(calc->floor_y);

    if (fx < 0) fx += 1.0;
    if (fy < 0) fy += 1.0;

    tex_coords[0] = (int)(tex->width * fx) & (tex->width - 1);
    tex_coords[1] = (int)(tex->height * fy) & (tex->height - 1);
}

static void calculate_ray_directions(t_game *game, double *ray_dir)
{
    // Calculate the leftmost and rightmost rays
    double camera_plane_x = game->player.plane_x;
    double camera_plane_y = game->player.plane_y;
    
    ray_dir[0] = game->player.dir_x - camera_plane_x;  // leftmost ray x
    ray_dir[1] = game->player.dir_y - camera_plane_y;  // leftmost ray y
    ray_dir[2] = game->player.dir_x + camera_plane_x;  // rightmost ray x
    ray_dir[3] = game->player.dir_y + camera_plane_y;  // rightmost ray y
}

static void get_ceiling_texture_coordinates(t_texture *tex, t_game *game, int x, int y, int *tex_coords)
{
    // For ceiling/sky, we want a more static effect that only changes with rotation
    double dir_x = game->player.dir_x;
    double dir_y = game->player.dir_y;
    
    // Calculate angle from player's direction
    double angle = atan2(dir_y, dir_x);
    if (angle < 0) angle += 2 * M_PI;
    
    // Map screen coordinates to texture coordinates
    double tx = (angle / (2 * M_PI) + (double)x / WINDOW_WIDTH) * tex->width;
    double ty = ((double)y / (WINDOW_HEIGHT / 2)) * tex->height;
    
    tex_coords[0] = ((int)tx) & (tex->width - 1);
    tex_coords[1] = ((int)ty) & (tex->height - 1);
}

static void draw_textured_floor_ceiling(t_game *game, int x, int y, int is_ceiling)
{
    double ray_dir[4];
    t_floor_calc calc;
    int tex_coords[2];
    t_texture *tex;

    if (is_ceiling == 1)
    {
        tex = &game->textures[TEX_CEIL];
        get_ceiling_texture_coordinates(tex, game, x, y, tex_coords);
    }
    else
    {
        calc.player_x = game->player.x;
        calc.player_y = game->player.y;
        calculate_ray_directions(game, ray_dir);
        calculate_floor_position(ray_dir, x, y, &calc);
        tex = &game->textures[TEX_FLOOR];
        get_texture_coordinates(tex, &calc, tex_coords);
    }

    unsigned int color = get_texture_color(tex, tex_coords[0], tex_coords[1]);
    my_mlx_pixel_put(game, x, y, color);
}

static void draw_ceiling_section(t_game *game, int x, int y)
{
    if (game->use_texture_ceiling == 1)
    {
        draw_textured_floor_ceiling(game, x, y, 1);
        return;
    }
    
    int ceiling_color;
    ceiling_color = (game->ceiling_color.r << 16);
    ceiling_color |= (game->ceiling_color.g << 8);
    ceiling_color |= game->ceiling_color.b;
    my_mlx_pixel_put(game, x, y, ceiling_color);
}

static void draw_floor_section(t_game *game, int x, int y)
{
    if (game->use_texture_floor == 1)
    {
        draw_textured_floor_ceiling(game, x, y, 0);
        return;
    }
    
    int floor_color;
    floor_color = (game->floor_color.r << 16);
    floor_color |= (game->floor_color.g << 8);
    floor_color |= game->floor_color.b;
    my_mlx_pixel_put(game, x, y, floor_color);
}

static void draw_ceiling_floor(t_game *game)
{
    int y;
    int x;

    y = 0;
    while (y < WINDOW_HEIGHT)
    {
        x = WINDOW_WIDTH / 2;
        while (x < WINDOW_WIDTH)
        {
            if (y < WINDOW_HEIGHT / 2)
            {
                draw_ceiling_section(game, x, y);
            }
            else
            {
                draw_floor_section(game, x, y);
            }
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
