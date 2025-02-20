/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:51:45 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/20 15:53:55 by iunikel          ###   ########.fr       */
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

static int check_collision(t_game *game, double x, double y)
{
    double radius;
    double buffer;
    int i;
    double angle;
    double check_x;
    double check_y;

    radius = game->player.hitbox_radius;
    buffer = game->player.wall_buffer;
    
    i = 0;
    while (i < 8)
    {
        angle = i * M_PI / 4;
        check_x = x + (radius + buffer) * cos(angle);
        check_y = y + (radius + buffer) * sin(angle);
        
        if (is_wall(game, check_x, check_y))
            return (1);
        i++;
    }
    return (0);
}

static void handle_movement(t_game *game)
{
    double new_x;
    double new_y;
    double move_x;
    double move_y;

    // Calculate movement vector based on input
    move_x = 0;
    move_y = 0;

    // Forward/Backward movement
    if (game->player.move_w)
    {
        move_x += game->player.dir_x * MOVE_SPEED;
        move_y += game->player.dir_y * MOVE_SPEED;
    }
    if (game->player.move_s)
    {
        move_x -= game->player.dir_x * MOVE_SPEED;
        move_y -= game->player.dir_y * MOVE_SPEED;
    }

    // Strafe movement
    if (game->player.move_a)
    {
        move_x += game->player.plane_x * -MOVE_SPEED;
        move_y += game->player.plane_y * -MOVE_SPEED;
    }
    if (game->player.move_d)
    {
        move_x += game->player.plane_x * MOVE_SPEED;
        move_y += game->player.plane_y * MOVE_SPEED;
    }

    // Try movement
    new_x = game->player.x + move_x;
    new_y = game->player.y + move_y;

    // Try diagonal movement first
    if (!check_collision(game, new_x, new_y))
    {
        game->player.x = new_x;
        game->player.y = new_y;
        return;
    }

    // If collision detected, try sliding along walls
    if (!check_collision(game, game->player.x + move_x, game->player.y))
        game->player.x += move_x;
    if (!check_collision(game, game->player.x, game->player.y + move_y))
        game->player.y += move_y;
}

static void handle_rotation(t_game *game)
{
    double old_dir_x;
    double old_plane_x;
    double rot_speed;

    if (!game->player.rot_left && !game->player.rot_right)
        return;

    rot_speed = ROT_SPEED * (game->player.rot_left ? -1 : 1);
    
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

void move_player(t_game *game)
{
    handle_movement(game);
    handle_rotation(game);
}
