/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:51:45 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/18 15:56:20 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static int	is_wall(t_game *game, double x, double y)
{
    int map_x;
    int map_y;

    map_x = (int)x;
    map_y = (int)y;
    
    if (map_x < 0 || map_x >= game->map.width || 
        map_y < 0 || map_y >= game->map.height)
        return (1);
    return (game->map.map[map_y][map_x] == '1');
}

static void	rotate_player(t_game *game, double rot_speed)
{
    double old_dir_x;
    double old_plane_x;

    old_dir_x = game->player.dir_x;
    game->player.dir_x = game->player.dir_x * cos(rot_speed) - 
                        game->player.dir_y * sin(rot_speed);
    game->player.dir_y = old_dir_x * sin(rot_speed) + 
                        game->player.dir_y * cos(rot_speed);
    
    old_plane_x = game->player.plane_x;
    game->player.plane_x = game->player.plane_x * cos(rot_speed) - 
                          game->player.plane_y * sin(rot_speed);
    game->player.plane_y = old_plane_x * sin(rot_speed) + 
                          game->player.plane_y * cos(rot_speed);
}

void	move_player(t_game *game)
{
    double new_x;
    double new_y;

    // Forward/Backward movement
    if (game->player.move_w)
    {
        new_x = game->player.x + game->player.dir_x * MOVE_SPEED;
        new_y = game->player.y + game->player.dir_y * MOVE_SPEED;
        if (!is_wall(game, new_x, game->player.y))
            game->player.x = new_x;
        if (!is_wall(game, game->player.x, new_y))
            game->player.y = new_y;
    }
    if (game->player.move_s)
    {
        new_x = game->player.x - game->player.dir_x * MOVE_SPEED;
        new_y = game->player.y - game->player.dir_y * MOVE_SPEED;
        if (!is_wall(game, new_x, game->player.y))
            game->player.x = new_x;
        if (!is_wall(game, game->player.x, new_y))
            game->player.y = new_y;
    }

    // Strafe movement
    if (game->player.move_a)
    {
        new_x = game->player.x - game->player.plane_x * MOVE_SPEED;
        new_y = game->player.y - game->player.plane_y * MOVE_SPEED;
        if (!is_wall(game, new_x, game->player.y))
            game->player.x = new_x;
        if (!is_wall(game, game->player.x, new_y))
            game->player.y = new_y;
    }
    if (game->player.move_d)
    {
        new_x = game->player.x + game->player.plane_x * MOVE_SPEED;
        new_y = game->player.y + game->player.plane_y * MOVE_SPEED;
        if (!is_wall(game, new_x, game->player.y))
            game->player.x = new_x;
        if (!is_wall(game, game->player.x, new_y))
            game->player.y = new_y;
    }

    // Rotation
    if (game->player.rot_left)
        rotate_player(game, -ROT_SPEED);
    if (game->player.rot_right)
        rotate_player(game, ROT_SPEED);
} 