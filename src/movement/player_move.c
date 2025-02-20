/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:51:45 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/20 12:13:34 by iunikel          ###   ########.fr       */
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
    
    // Check multiple points around the hitbox circle
    i = 0;
    while (i < 8)
    {
        angle = i * M_PI / 4;  // Check 8 points around the circle
        check_x = x + (radius + buffer) * cos(angle);
        check_y = y + (radius + buffer) * sin(angle);
        
        if (is_wall(game, check_x, check_y))
            return (1);
        i++;
    }
    return (0);
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

static void	handle_forward_backward(t_game *game)
{
    double new_x;
    double new_y;
    double move_step;
    double slide_x;
    double slide_y;

    move_step = MOVE_SPEED;
    if (game->player.move_w)
        move_step = MOVE_SPEED;
    else if (game->player.move_s)
        move_step = -MOVE_SPEED;
    else
        return;

    new_x = game->player.x + game->player.dir_x * move_step;
    new_y = game->player.y + game->player.dir_y * move_step;

    // Try diagonal movement first
    if (!check_collision(game, new_x, new_y))
    {
        game->player.x = new_x;
        game->player.y = new_y;
        return;
    }

    // If collision detected, try sliding along walls
    slide_x = game->player.x + game->player.dir_x * move_step;
    if (!check_collision(game, slide_x, game->player.y))
        game->player.x = slide_x;

    slide_y = game->player.y + game->player.dir_y * move_step;
    if (!check_collision(game, game->player.x, slide_y))
        game->player.y = slide_y;
}

static void	handle_strafe(t_game *game)
{
    double new_x;
    double new_y;
    double move_step;
    double slide_x;
    double slide_y;

    move_step = MOVE_SPEED;
    if (game->player.move_a)
        move_step = -MOVE_SPEED;
    else if (game->player.move_d)
        move_step = MOVE_SPEED;
    else
        return;

    new_x = game->player.x + game->player.plane_x * move_step;
    new_y = game->player.y + game->player.plane_y * move_step;

    // Try diagonal movement first
    if (!check_collision(game, new_x, new_y))
    {
        game->player.x = new_x;
        game->player.y = new_y;
        return;
    }

    // If collision detected, try sliding along walls
    slide_x = game->player.x + game->player.plane_x * move_step;
    if (!check_collision(game, slide_x, game->player.y))
        game->player.x = slide_x;

    slide_y = game->player.y + game->player.plane_y * move_step;
    if (!check_collision(game, game->player.x, slide_y))
        game->player.y = slide_y;
}

static void	handle_rotation(t_game *game)
{
    if (game->player.rot_left)
        rotate_player(game, -ROT_SPEED);
    if (game->player.rot_right)
        rotate_player(game, ROT_SPEED);
}

void	move_player(t_game *game)
{
    handle_forward_backward(game);
    handle_strafe(game);
    handle_rotation(game);
}
