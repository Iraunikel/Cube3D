/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:44:11 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/20 21:56:10 by iunikel          ###   ########.fr       */
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
	start_x = x * size + (WINDOW_WIDTH / 4 - (game->map.width * size) / 2);
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
				draw_square(game, x, y, 0xFFFFFF);
			else if (game->map.map[y][x] == '0' || ft_strchr("NSEW",
					game->map.map[y][x]))
				draw_square(game, x, y, 0x333333);
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
			return (&game->textures[2]);
		return (&game->textures[3]);
	}
	if (ray->dir_y > 0)
		return (&game->textures[1]);
	return (&game->textures[0]);
}

static void calculate_wall_x(t_ray *ray, t_player *player)
{
	if (ray->side == 0)
		ray->wall_x = player->y + ray->perp_wall_dist * ray->dir_y;
	else
		ray->wall_x = player->x + ray->perp_wall_dist * ray->dir_x;
	ray->wall_x -= floor(ray->wall_x);
}

static void calculate_tex_x(t_ray *ray, t_texture *tex, int *tex_x)
{
	*tex_x = (int)(ray->wall_x * (double)tex->width);
	if ((ray->side == 0 && ray->dir_x > 0) || (ray->side == 1 && ray->dir_y < 0))
		*tex_x = tex->width - *tex_x - 1;
	if (*tex_x < 0)
		*tex_x = 0;
	if (*tex_x >= tex->width)
		*tex_x = tex->width - 1;
}

static void calculate_tex_pos(int line_height, int draw_start, double *tex_pos, double step)
{
	*tex_pos = (draw_start - WINDOW_HEIGHT / 2 + line_height / 2) * step;
	*tex_pos += 0.00001;
}

static double calculate_step(int line_height, int tex_height)
{
	return 1.0 * (double)tex_height / (double)line_height;
}

static void apply_lighting_factors(int *r, int *g, int *b, double dist_factor)
{
	*r = (int)(*r * dist_factor);
	*g = (int)(*g * dist_factor);
	*b = (int)(*b * dist_factor);
	if (*r < 0)
		*r = 0;
	if (*r > 255)
		*r = 255;
	if (*g < 0)
		*g = 0;
	if (*g > 255)
		*g = 255;
	if (*b < 0)
		*b = 0;
	if (*b > 255)
		*b = 255;
}

static void calculate_dist_factor(t_ray *ray, double *dist_factor, int y)
{
	*dist_factor = 1.0;
	if (ray->perp_wall_dist > 1.0)
	{
		*dist_factor = 1.0 / (1.0 + (ray->perp_wall_dist - 1.0) * 0.5);
		*dist_factor = fmax(0.2, *dist_factor);
	}
	if (ray->side == 1)
	{
		double side_factor = 0.6 + 0.4 * (1.0 - fabs(ray->dir_y));
		*dist_factor *= side_factor;
	}
	double height_factor = 1.0 - fabs((double)(y - WINDOW_HEIGHT / 2) / (double)WINDOW_HEIGHT) * 0.2;
	*dist_factor *= height_factor;
	*dist_factor *= 0.85;
}

static void clamp_tex_y(int *tex_y, int height)
{
    if (*tex_y < 0)
        *tex_y = 0;
    if (*tex_y >= height)
        *tex_y = height - 1;
}

static void extract_rgb(unsigned int color, int *r, int *g, int *b)
{
    *r = (color >> 16) & 0xFF;
    *g = (color >> 8) & 0xFF;
    *b = color & 0xFF;
}

static unsigned int combine_rgb(int r, int g, int b)
{
    return (r << 16) | (g << 8) | b;
}

static void process_wall_pixel(t_wall_render *render, t_wall_data *data)
{
    int tex_y;
    unsigned int color;
    int rgb[3];
    double dist_factor;

    tex_y = (int)data->tex_pos & (render->tex->height - 1);
    clamp_tex_y(&tex_y, render->tex->height);
    color = get_texture_color(render->tex, data->tex_x, tex_y);
    extract_rgb(color, &rgb[0], &rgb[1], &rgb[2]);
    calculate_dist_factor(render->ray, &dist_factor, data->y);
    apply_lighting_factors(&rgb[0], &rgb[1], &rgb[2], dist_factor);
    color = combine_rgb(rgb[0], rgb[1], rgb[2]);
    my_mlx_pixel_put(render->game, data->x, data->y, color);
}

void draw_wall_segment(t_game *game, t_ray *ray, t_texture *tex, t_wall_data *data)
{
    t_wall_render render;

    render.game = game;
    render.ray = ray;
    render.tex = tex;
    data->y = data->draw_start;
    while (data->y < data->draw_end)
    {
        process_wall_pixel(&render, data);
        data->tex_pos += data->step;
        data->y++;
    }
}

void	draw_textured_wall(t_game *game, t_ray *ray, int x)
{
	t_texture   *tex;
	t_wall_data data;
	int         line_height;

	tex = get_wall_texture(game, ray);
	calculate_wall_x(ray, &game->player);
	ray->wall_x -= floor(ray->wall_x);
	ray->wall_x = fmax(0.0, fmin(1.0, ray->wall_x));
	calculate_tex_x(ray, tex, &data.tex_x);
	calculate_line_dimensions(ray, &line_height, &data.draw_start, &data.draw_end);
	data.step = calculate_step(line_height, tex->height);
	calculate_tex_pos(line_height, data.draw_start, &data.tex_pos, data.step);
	data.x = x;
	draw_wall_segment(game, ray, tex, &data);
}

void	draw_3d_view(t_game *game, t_ray *ray, int x)
{
	int screen_x;

	screen_x = x + WINDOW_WIDTH / 2;
	draw_textured_wall(game, ray, screen_x);
}

static void calculate_floor_position(double *ray_dir, int x, int y, t_floor_calc *calc)
{
    double ray_dir_x0 = ray_dir[0];
    double ray_dir_y0 = ray_dir[1];
    double ray_dir_x1 = ray_dir[2];
    double ray_dir_y1 = ray_dir[3];
    double p = y - WINDOW_HEIGHT / 2.0;
    double pos_z = 0.5 * WINDOW_HEIGHT;
    double row_distance = pos_z / p;
    double floor_step_x = row_distance * (ray_dir_x1 - ray_dir_x0) / WINDOW_WIDTH;
    double floor_step_y = row_distance * (ray_dir_y1 - ray_dir_y0) / WINDOW_WIDTH;
    calc->floor_x = calc->player_x + row_distance * ray_dir_x0;
    calc->floor_y = calc->player_y + row_distance * ray_dir_y0;
    double relative_x = x - WINDOW_WIDTH / 2.0;
    calc->floor_x += floor_step_x * relative_x;
    calc->floor_y += floor_step_y * relative_x;
}

static void get_texture_coordinates(t_texture *tex, t_floor_calc *calc, int *tex_coords)
{
    double fx = calc->floor_x - floor(calc->floor_x);
    double fy = calc->floor_y - floor(calc->floor_y);
    if (fx < 0)
		fx += 1.0;
    if (fy < 0)
		fy += 1.0;
    tex_coords[0] = (int)(tex->width * fx) & (tex->width - 1);
    tex_coords[1] = (int)(tex->height * fy) & (tex->height - 1);
}

static void calculate_ray_directions(t_game *game, double *ray_dir)
{
    double camera_plane_x = game->player.plane_x;
    double camera_plane_y = game->player.plane_y;
    ray_dir[0] = game->player.dir_x - camera_plane_x;
    ray_dir[1] = game->player.dir_y - camera_plane_y;
    ray_dir[2] = game->player.dir_x + camera_plane_x;
    ray_dir[3] = game->player.dir_y + camera_plane_y;
}

static void get_ceiling_texture_coordinates(t_texture *tex, t_game *game, int x, int y, int *tex_coords)
{
    double dir_x = game->player.dir_x;
    double dir_y = game->player.dir_y;
    double angle = atan2(dir_y, dir_x);
    if (angle < 0)
		angle += 2 * M_PI;
    double tx = (angle / (2 * M_PI) + (double)x / WINDOW_WIDTH) * tex->width;
    double ty = ((double)y / (WINDOW_HEIGHT / 2)) * tex->height;
    tex_coords[0] = ((int)tx) & (tex->width - 1);
    tex_coords[1] = ((int)ty) & (tex->height - 1);
}

static void apply_distance_darkening(int *r, int *g, int *b, int y, int is_ceiling)
{
    if (!is_ceiling)
    {
        double relative_y = (double)(y - WINDOW_HEIGHT / 2) / (WINDOW_HEIGHT / 2);
        double floor_dist = 1.0 - relative_y;
        double dist_factor = 1.0 - (floor_dist * 0.7);
        dist_factor = fmax(0.2, dist_factor);
        dist_factor *= 0.85;
        *r = (int)(*r * dist_factor);
        *g = (int)(*g * dist_factor);
        *b = (int)(*b * dist_factor);
        if (*r < 0)
			*r = 0;
        if (*r > 255)
			*r = 255;
        if (*g < 0)
			*g = 0;
        if (*g > 255)
			*g = 255;
        if (*b < 0)
			*b = 0;
        if (*b > 255)
			*b = 255;
    }
}

static void get_texture_and_coordinates(t_game *game, int x, int y, int is_ceiling, t_texture **tex, int *tex_coords)
{
    double ray_dir[4];
    t_floor_calc calc;

    if (is_ceiling == 1)
    {
        *tex = &game->textures[TEX_CEIL];
        get_ceiling_texture_coordinates(*tex, game, x, y, tex_coords);
    }
    else
    {
        calc.player_x = game->player.x;
        calc.player_y = game->player.y;
        calculate_ray_directions(game, ray_dir);
        calculate_floor_position(ray_dir, x, y, &calc);
        *tex = &game->textures[TEX_FLOOR];
        get_texture_coordinates(*tex, &calc, tex_coords);
    }
}

static void apply_color_and_draw(t_game *game, int x, int y, int is_ceiling, t_texture *tex, int *tex_coords)
{
    unsigned int color = get_texture_color(tex, tex_coords[0], tex_coords[1]);
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;
    apply_distance_darkening(&r, &g, &b, y, is_ceiling);
    color = (r << 16) | (g << 8) | b;
    my_mlx_pixel_put(game, x, y, color);
}

static void draw_textured_floor_ceiling(t_game *game, int x, int y, int is_ceiling)
{
    t_texture *tex;
    int tex_coords[2];

    get_texture_and_coordinates(game, x, y, is_ceiling, &tex, tex_coords);
    apply_color_and_draw(game, x, y, is_ceiling, tex, tex_coords);
}

static void draw_ceiling_section(t_game *game, int x, int y)
{
    if (game->use_texture_ceiling == 1)
    {
        draw_textured_floor_ceiling(game, x, y, 1);
        return;
    }
    int r = game->ceiling_color.r;
    int g = game->ceiling_color.g;
    int b = game->ceiling_color.b;
    apply_distance_darkening(&r, &g, &b, y, 1);
    int ceiling_color = (r << 16) | (g << 8) | b;
    my_mlx_pixel_put(game, x, y, ceiling_color);
}

static void draw_floor_section(t_game *game, int x, int y)
{
    if (game->use_texture_floor == 1)
    {
        draw_textured_floor_ceiling(game, x, y, 0);
        return;
    }
    int r = game->floor_color.r;
    int g = game->floor_color.g;
    int b = game->floor_color.b;
    apply_distance_darkening(&r, &g, &b, y, 0);
    int floor_color = (r << 16) | (g << 8) | b;
    my_mlx_pixel_put(game, x, y, floor_color);
}

static void process_ceiling_floor_row(t_game *game, int y)
{
    int x;

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
}

static void draw_ceiling_floor(t_game *game)
{
    int y;

    y = 0;
    while (y < WINDOW_HEIGHT)
    {
        process_ceiling_floor_row(game, y);
        y++;
    }
}

int	game_loop(t_game *game)
{
	static int		frame_count = 0;
	static clock_t	last_time = 0;
	clock_t			current_time;

	ft_bzero(game->addr, WINDOW_WIDTH * WINDOW_HEIGHT * (game->bits_per_pixel / 8));
	move_player(game);
	draw_ceiling_floor(game);
	draw_map(game);
	cast_rays(game);
	frame_count++;
	current_time = clock();
	if (current_time - last_time >= CLOCKS_PER_SEC)
	{
		printf("FPS: %d\n", frame_count);
		frame_count = 0;
		last_time = current_time;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}
