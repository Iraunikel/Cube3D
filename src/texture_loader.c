/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_loader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:49:14 by iunikel           #+#    #+#             */
/*   Updated: 2025/03/08 21:30:30 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

/*
static void init_texture(t_texture *tex, void *mlx, char *path)
{
    printf("Loading texture from path: %s\n", path);
    tex->img = mlx_xpm_file_to_image(mlx, path, &tex->width, &tex->height);
    if (!tex->img)
        error_exit("Failed to load texture");
    tex->addr = mlx_get_data_addr(tex->img, &tex->bits_per_pixel,
        &tex->line_length, &tex->endian);
    if (!tex->addr)
        error_exit("Failed to get texture address");
}
*/

void    load_textures(t_game *game)
{
    printf("Debug - Texture paths:\n");
    printf("North: %s\n", game->north_texture);
    printf("South: %s\n", game->south_texture);
    printf("West: %s\n", game->west_texture);
    printf("East: %s\n", game->east_texture);
    printf("Floor: %s (Use texture: %d)\n", game->floor_texture, game->use_texture_floor);
    printf("Ceiling: %s (Use texture: %d)\n", game->ceiling_texture, game->use_texture_ceiling);

    if (!game->north_texture || !game->south_texture || 
        !game->west_texture || !game->east_texture)
        error_exit("Missing wall texture path");

    // Load and verify wall textures
    game->textures[TEX_NORTH].img = mlx_xpm_file_to_image(game->mlx, 
        game->north_texture, 
        &game->textures[TEX_NORTH].width, 
        &game->textures[TEX_NORTH].height);
    if (!game->textures[TEX_NORTH].img || 
        game->textures[TEX_NORTH].width != TEX_WIDTH || 
        game->textures[TEX_NORTH].height != TEX_HEIGHT)
        error_exit("North texture dimensions must be TEX_WIDTH x TEX_HEIGHT");

    game->textures[TEX_SOUTH].img = mlx_xpm_file_to_image(game->mlx, 
        game->south_texture, 
        &game->textures[TEX_SOUTH].width, 
        &game->textures[TEX_SOUTH].height);
    if (!game->textures[TEX_SOUTH].img || 
        game->textures[TEX_SOUTH].width != TEX_WIDTH || 
        game->textures[TEX_SOUTH].height != TEX_HEIGHT)
        error_exit("South texture dimensions must be TEX_WIDTH x TEX_HEIGHT");

    game->textures[TEX_EAST].img = mlx_xpm_file_to_image(game->mlx, 
        game->east_texture, 
        &game->textures[TEX_EAST].width, 
        &game->textures[TEX_EAST].height);
    if (!game->textures[TEX_EAST].img || 
        game->textures[TEX_EAST].width != TEX_WIDTH || 
        game->textures[TEX_EAST].height != TEX_HEIGHT)
        error_exit("East texture dimensions must be TEX_WIDTH x TEX_HEIGHT");

    game->textures[TEX_WEST].img = mlx_xpm_file_to_image(game->mlx, 
        game->west_texture, 
        &game->textures[TEX_WEST].width, 
        &game->textures[TEX_WEST].height);
    if (!game->textures[TEX_WEST].img || 
        game->textures[TEX_WEST].width != TEX_WIDTH || 
        game->textures[TEX_WEST].height != TEX_HEIGHT)
        error_exit("West texture dimensions must be TEX_WIDTH x TEX_HEIGHT");

    // Get data addresses for all wall textures
    game->textures[TEX_NORTH].addr = mlx_get_data_addr(game->textures[TEX_NORTH].img,
        &game->textures[TEX_NORTH].bits_per_pixel,
        &game->textures[TEX_NORTH].line_length,
        &game->textures[TEX_NORTH].endian);
    if (!game->textures[TEX_NORTH].addr)
        error_exit("Failed to get North texture address");

    game->textures[TEX_SOUTH].addr = mlx_get_data_addr(game->textures[TEX_SOUTH].img,
        &game->textures[TEX_SOUTH].bits_per_pixel,
        &game->textures[TEX_SOUTH].line_length,
        &game->textures[TEX_SOUTH].endian);
    if (!game->textures[TEX_SOUTH].addr)
        error_exit("Failed to get South texture address");

    game->textures[TEX_EAST].addr = mlx_get_data_addr(game->textures[TEX_EAST].img,
        &game->textures[TEX_EAST].bits_per_pixel,
        &game->textures[TEX_EAST].line_length,
        &game->textures[TEX_EAST].endian);
    if (!game->textures[TEX_EAST].addr)
        error_exit("Failed to get East texture address");

    game->textures[TEX_WEST].addr = mlx_get_data_addr(game->textures[TEX_WEST].img,
        &game->textures[TEX_WEST].bits_per_pixel,
        &game->textures[TEX_WEST].line_length,
        &game->textures[TEX_WEST].endian);
    if (!game->textures[TEX_WEST].addr)
        error_exit("Failed to get West texture address");

    // Load optional floor and ceiling textures if enabled
    if (game->use_texture_floor)
    {
        if (!game->floor_texture)
            error_exit("Floor texture enabled but no path specified");
        game->textures[TEX_FLOOR].img = mlx_xpm_file_to_image(game->mlx,
            game->floor_texture,
            &game->textures[TEX_FLOOR].width,
            &game->textures[TEX_FLOOR].height);
        if (!game->textures[TEX_FLOOR].img || 
            game->textures[TEX_FLOOR].width != TEX_WIDTH || 
            game->textures[TEX_FLOOR].height != TEX_HEIGHT)
            error_exit("Floor texture dimensions must be TEX_WIDTH x TEX_HEIGHT");
        game->textures[TEX_FLOOR].addr = mlx_get_data_addr(game->textures[TEX_FLOOR].img,
            &game->textures[TEX_FLOOR].bits_per_pixel,
            &game->textures[TEX_FLOOR].line_length,
            &game->textures[TEX_FLOOR].endian);
        if (!game->textures[TEX_FLOOR].addr)
            error_exit("Failed to get Floor texture address");
    }

    if (game->use_texture_ceiling)
    {
        if (!game->ceiling_texture)
            error_exit("Ceiling texture enabled but no path specified");
        game->textures[TEX_CEIL].img = mlx_xpm_file_to_image(game->mlx,
            game->ceiling_texture,
            &game->textures[TEX_CEIL].width,
            &game->textures[TEX_CEIL].height);
        if (!game->textures[TEX_CEIL].img || 
            game->textures[TEX_CEIL].width != TEX_WIDTH || 
            game->textures[TEX_CEIL].height != TEX_HEIGHT)
            error_exit("Ceiling texture dimensions must be TEX_WIDTH x TEX_HEIGHT");
        game->textures[TEX_CEIL].addr = mlx_get_data_addr(game->textures[TEX_CEIL].img,
            &game->textures[TEX_CEIL].bits_per_pixel,
            &game->textures[TEX_CEIL].line_length,
            &game->textures[TEX_CEIL].endian);
        if (!game->textures[TEX_CEIL].addr)
            error_exit("Failed to get Ceiling texture address");
    }
}

unsigned int get_texture_color(t_texture *tex, int x, int y)
{
    char    *pixel;
    
    // Use bit masking for faster bounds handling (assumes texture dimensions are powers of 2)
    x = x & (tex->width - 1);
    y = y & (tex->height - 1);
    
    // Calculate pixel offset directly without additional bounds checking
    pixel = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8));
    return (*(unsigned int *)pixel);
} 