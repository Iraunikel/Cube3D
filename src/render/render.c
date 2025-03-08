/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:44:11 by iunikel           #+#    #+#             */
/*   Updated: 2025/03/08 21:30:02 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static void	my_mlx_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	// Skip bounds checking for most cases since we're iterating within bounds
	// Only check for edge cases
	if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
		return ;
	dst = game->addr + (y * game->line_length + x * (game->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
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
	// Render directly to the x coordinate without offset
	draw_textured_wall(game, ray, x);
}

static void get_texture_coordinates(t_texture *tex, t_floor_calc *calc, int *tex_coords)
{
    // Get the exact integer cell the ray is in
    int cell_x = (int)(calc->floor_x);
    int cell_y = (int)(calc->floor_y);
    
    // Get the exact position inside the cell (0-1 value)
    double fx = calc->floor_x - cell_x;
    double fy = calc->floor_y - cell_y;
    
    // Convert to texture coordinates
    tex_coords[0] = (int)(fx * tex->width);
    tex_coords[1] = (int)(fy * tex->height);
    
    // Ensure we stay within texture bounds
    tex_coords[0] = (tex_coords[0] + tex->width) % tex->width;
    tex_coords[1] = (tex_coords[1] + tex->height) % tex->height;
}

static void get_ceiling_texture_coordinates(t_texture *tex, t_game *game, int x, int y, int *tex_coords)
{
    static double last_dir_x = 0.0;
    static double last_dir_y = 0.0;
    static double last_angle = 0.0;
    static int last_tex_width = 0;
    
    // Only recalculate angle when direction changes
    if (game->player.dir_x != last_dir_x || game->player.dir_y != last_dir_y || tex->width != last_tex_width)
    {
        last_dir_x = game->player.dir_x;
        last_dir_y = game->player.dir_y;
        last_tex_width = tex->width;
        last_angle = atan2(last_dir_y, last_dir_x);
        if (last_angle < 0)
            last_angle += 2 * M_PI;
    }
    
    // Simplified calculation using precomputed angle
    double tx = (last_angle / (2 * M_PI) + (double)x / WINDOW_WIDTH) * tex->width;
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
    static t_floor_calc calc;
    static double last_y = -1;
    static int last_is_ceiling = -1;
    static double ray_dir_z = 0.0;
    static double precomputed_camera_x[WINDOW_WIDTH];
    static double precomputed_ray_dir_x[WINDOW_WIDTH];
    static double precomputed_ray_dir_y[WINDOW_WIDTH];
    static int initialized = 0;
    
    // Initialize precomputed values once
    if (!initialized)
    {
        int i = 0;
        while (i < WINDOW_WIDTH)
        {
            precomputed_camera_x[i] = 2.0 * i / (double)WINDOW_WIDTH - 1.0;
            i++;
        }
        initialized = 1;
    }

    if (is_ceiling == 1)
    {
        *tex = &game->textures[TEX_CEIL];
        get_ceiling_texture_coordinates(*tex, game, x, y, tex_coords);
    }
    else
    {
        // Only recalculate when y changes or player direction changes
        if (y != last_y || last_is_ceiling != is_ceiling)
        {
            calc.game = game;
            
            // Current y position compared to the center of the screen
            ray_dir_z = (WINDOW_HEIGHT / 2.0) / (y - WINDOW_HEIGHT / 2.0);
            
            // Precompute ray directions for all x values
            int i = 0;
            while (i < WINDOW_WIDTH)
            {
                precomputed_ray_dir_x[i] = game->player.dir_x + game->player.plane_x * precomputed_camera_x[i];
                precomputed_ray_dir_y[i] = game->player.dir_y + game->player.plane_y * precomputed_camera_x[i];
                i++;
            }
            
            last_y = y;
            last_is_ceiling = is_ceiling;
        }
        
        // Use precomputed values
        calc.floor_x = game->player.x + precomputed_ray_dir_x[x] * ray_dir_z;
        calc.floor_y = game->player.y + precomputed_ray_dir_y[x] * ray_dir_z;
        
        *tex = &game->textures[TEX_FLOOR];
        get_texture_coordinates(*tex, &calc, tex_coords);
    }
}

static void apply_color_and_draw(t_game *game, int x, int y, int is_ceiling, t_texture *tex, int *tex_coords)
{
    static int last_y = -1;
    static int last_is_ceiling = -1;
    static double darkening_factor = 1.0;
    
    // Only recalculate darkening factor when y or is_ceiling changes
    if (y != last_y || is_ceiling != last_is_ceiling)
    {
        if (!is_ceiling)
        {
            double relative_y = (double)(y - WINDOW_HEIGHT / 2) / (WINDOW_HEIGHT / 2);
            double floor_dist = 1.0 - relative_y;
            darkening_factor = 1.0 - (floor_dist * 0.7);
            darkening_factor = fmax(0.2, darkening_factor);
            darkening_factor *= 0.85;
        }
        else
        {
            darkening_factor = 1.0;
        }
        
        last_y = y;
        last_is_ceiling = is_ceiling;
    }
    
    unsigned int color = get_texture_color(tex, tex_coords[0], tex_coords[1]);
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;
    
    // Apply precomputed darkening factor
    r = (int)(r * darkening_factor);
    g = (int)(g * darkening_factor);
    b = (int)(b * darkening_factor);
    
    // Clamp values
    r = (r < 0) ? 0 : ((r > 255) ? 255 : r);
    g = (g < 0) ? 0 : ((g > 255) ? 255 : g);
    b = (b < 0) ? 0 : ((b > 255) ? 255 : b);
    
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

static void process_ceiling_floor_row(t_game *game, int y)
{
    int x;
    int r, g, b;
    int ceiling_color, floor_color;
    int is_ceiling = (y < WINDOW_HEIGHT / 2);
    t_texture *tex;
    int tex_coords[2];
    
    // Precompute colors for solid color floor/ceiling
    if (!game->use_texture_ceiling && is_ceiling)
    {
        r = game->ceiling_color.r;
        g = game->ceiling_color.g;
        b = game->ceiling_color.b;
        apply_distance_darkening(&r, &g, &b, y, 1);
        ceiling_color = (r << 16) | (g << 8) | b;
    }
    
    if (!game->use_texture_floor && !is_ceiling)
    {
        r = game->floor_color.r;
        g = game->floor_color.g;
        b = game->floor_color.b;
        apply_distance_darkening(&r, &g, &b, y, 0);
        floor_color = (r << 16) | (g << 8) | b;
    }
    
    // Process ceiling
    if (is_ceiling)
    {
        if (game->use_texture_ceiling)
        {
            // For textured ceiling, process in batches
            tex = &game->textures[TEX_CEIL];
            x = 0;
            while (x < WINDOW_WIDTH)
            {
                get_ceiling_texture_coordinates(tex, game, x, y, tex_coords);
                apply_color_and_draw(game, x, y, 1, tex, tex_coords);
                x++;
            }
        }
        else
        {
            // For solid color ceiling, fill the entire row at once
            x = 0;
            while (x < WINDOW_WIDTH)
            {
                my_mlx_pixel_put(game, x, y, ceiling_color);
                x++;
            }
        }
    }
    // Process floor
    else
    {
        if (game->use_texture_floor)
        {
            // For textured floor, process in batches
            x = 0;
            while (x < WINDOW_WIDTH)
            {
                draw_textured_floor_ceiling(game, x, y, 0);
                x++;
            }
        }
        else
        {
            // For solid color floor, fill the entire row at once
            x = 0;
            while (x < WINDOW_WIDTH)
            {
                my_mlx_pixel_put(game, x, y, floor_color);
                x++;
            }
        }
    }
}

static void draw_ceiling_floor(t_game *game)
{
    int y;
    int half_height = WINDOW_HEIGHT / 2;

    // Process ceiling and floor in parallel
    y = 0;
    while (y < half_height)
    {
        // Process ceiling row
        process_ceiling_floor_row(game, y);
        
        // Process corresponding floor row
        process_ceiling_floor_row(game, WINDOW_HEIGHT - y - 1);
        
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
	// Removed draw_map(game) call to hide the 2D map
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
