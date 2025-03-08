/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 11:03:45 by iunikel           #+#    #+#             */
/*   Updated: 2025/03/08 11:03:53 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	is_wall(t_game *game, double x, double y)
{
	int	map_x;
	int	map_y;

	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_x >= game->map.width
		|| map_y < 0 || map_y >= game->map.height)
		return (1);
	return (game->map.map[map_y][map_x] == '1');
}

static int	check_circle_points(t_game *game, double x, double y,
	double total_radius)
{
	int		i;
	double	angle;
	double	check_x;
	double	check_y;

	i = 0;
	while (i < 8)
	{
		angle = i * M_PI / 4;
		check_x = x + total_radius * cos(angle);
		check_y = y + total_radius * sin(angle);
		if (is_wall(game, check_x, check_y))
			return (1);
		i++;
	}
	return (0);
}

int	check_collision(t_game *game, double x, double y)
{
	double	total_radius;

	if (is_wall(game, x, y))
		return (1);
	total_radius = game->player.hitbox_radius + game->player.wall_buffer;
	return (check_circle_points(game, x, y, total_radius));
}

static void	try_individual_slides(t_game *game, double move_x, double move_y,
	double slide_factor)
{
	double	slide_x;
	double	slide_y;

	slide_x = move_x * slide_factor;
	if (!check_collision(game, game->player.x + slide_x, game->player.y))
		game->player.x += slide_x;
	slide_y = move_y * slide_factor;
	if (!check_collision(game, game->player.x, game->player.y + slide_y))
		game->player.y += slide_y;
}

void	try_slide_movement(t_game *game, double move_x, double move_y)
{
	if (!check_collision(game, game->player.x + move_x, game->player.y + move_y))
	{
		game->player.x += move_x;
		game->player.y += move_y;
		return ;
	}
	try_individual_slides(game, move_x, move_y, 0.8);
} 