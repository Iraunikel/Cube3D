/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:51:45 by iunikel           #+#    #+#             */
/*   Updated: 2025/03/08 11:09:32 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

void	try_slide_movement(t_game *game, double move_x, double move_y);

static void	calculate_movement(t_game *game, double *move_x, double *move_y)
{
	*move_x = 0;
	*move_y = 0;
	if (game->player.move_w)
	{
		*move_x += game->player.dir_x * MOVE_SPEED;
		*move_y += game->player.dir_y * MOVE_SPEED;
	}
	if (game->player.move_s)
	{
		*move_x -= game->player.dir_x * MOVE_SPEED;
		*move_y -= game->player.dir_y * MOVE_SPEED;
	}
	if (game->player.move_a)
	{
		*move_x += game->player.plane_x * -MOVE_SPEED;
		*move_y += game->player.plane_y * -MOVE_SPEED;
	}
	if (game->player.move_d)
	{
		*move_x += game->player.plane_x * MOVE_SPEED;
		*move_y += game->player.plane_y * MOVE_SPEED;
	}
}

static void	handle_movement(t_game *game)
{
	double	move_x;
	double	move_y;

	calculate_movement(game, &move_x, &move_y);
	if (move_x != 0 || move_y != 0)
		try_slide_movement(game, move_x, move_y);
}

static void	handle_rotation(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;
	double	rot_speed;

	if (!game->player.rot_left && !game->player.rot_right)
		return ;
	rot_speed = ROT_SPEED * (game->player.rot_left ? -1 : 1);
	old_dir_x = game->player.dir_x;
	game->player.dir_x = game->player.dir_x * cos(rot_speed)
		- game->player.dir_y * sin(rot_speed);
	game->player.dir_y = old_dir_x * sin(rot_speed)
		+ game->player.dir_y * cos(rot_speed);
	old_plane_x = game->player.plane_x;
	game->player.plane_x = game->player.plane_x * cos(rot_speed)
		- game->player.plane_y * sin(rot_speed);
	game->player.plane_y = old_plane_x * sin(rot_speed)
		+ game->player.plane_y * cos(rot_speed);
}

void	move_player(t_game *game)
{
	handle_movement(game);
	handle_rotation(game);
}
