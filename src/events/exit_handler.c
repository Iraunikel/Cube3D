/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:43:33 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/19 15:37:42 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

static void	cleanup_map(t_game *game)
{
	int	i;

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
}

static void	cleanup_textures(t_game *game)
{
	if (game->north_texture)
		free(game->north_texture);
	if (game->south_texture)
		free(game->south_texture);
	if (game->west_texture)
		free(game->west_texture);
	if (game->east_texture)
		free(game->east_texture);
}

static void	cleanup_mlx(t_game *game)
{
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

void	cleanup_game(t_game *game)
{
	cleanup_map(game);
	cleanup_textures(game);
	cleanup_mlx(game);
}
