/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:43:33 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/17 22:43:40 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

void cleanup_game(t_game *game)
{
    int i;

    // Free map data
    if (game->map.map)
    {
        i = 0;
        while (i < game->map.height)
        {
            free(game->map.map[i]);
            i++;
        }
        free(game->map.map);
    }

    // Free texture paths
    if (game->north_texture)
        free(game->north_texture);
    if (game->south_texture)
        free(game->south_texture);
    if (game->west_texture)
        free(game->west_texture);
    if (game->east_texture)
        free(game->east_texture);

    // Cleanup MLX resources
    if (game->img)
        mlx_destroy_image(game->mlx, game->img);
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
    if (game->mlx)
    {
        free(game->mlx);
        game->mlx = NULL;
    }
} 