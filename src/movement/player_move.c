/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:51:45 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/20 13:06:58 by iunikel          ###   ########.fr       */
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

static void update_velocity(t_player *player)
{
    // Forward/Backward velocity
    if (player->move_w || player->move_s)
    {
        double target_velocity = player->move_w ? MAX_SPEED : -MAX_SPEED;
        if (player->velocity < target_velocity)
            player->velocity += ACCELERATION;
        else if (player->velocity > target_velocity)
            player->velocity -= ACCELERATION;
    }
    else
    {
        // Decelerate when no movement keys are pressed
        if (player->velocity > 0)
            player->velocity = fmax(0, player->velocity - DECELERATION);
        else if (player->velocity < 0)
            player->velocity = fmin(0, player->velocity + DECELERATION);
    }

    // Rotation velocity (both arrows and A/D keys)
    if (player->rot_right || player->move_d || player->rot_left || player->move_a)
    {
        double target_rot;
        if (player->rot_right || player->move_d)
            target_rot = MAX_ROT_SPEED;
        else
            target_rot = -MAX_ROT_SPEED;

        if (player->rot_velocity < target_rot)
            player->rot_velocity += ACCELERATION;
        else if (player->rot_velocity > target_rot)
            player->rot_velocity -= ACCELERATION;
    }
    else
    {
        // Decelerate rotation when no keys are pressed
        if (player->rot_velocity > 0)
            player->rot_velocity = fmax(0, player->rot_velocity - DECELERATION);
        else if (player->rot_velocity < 0)
            player->rot_velocity = fmin(0, player->rot_velocity + DECELERATION);
    }
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

static void handle_forward_backward(t_game *game)
{
    double new_x;
    double new_y;
    double move_step;
    double slide_x;
    double slide_y;

    if (game->player.velocity == 0)
        return;

    move_step = game->player.velocity;
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

static void handle_rotation(t_game *game)
{
    if (game->player.rot_velocity == 0)
        return;
        
    double old_dir_x = game->player.dir_x;
    game->player.dir_x = game->player.dir_x * cos(game->player.rot_velocity) - 
                        game->player.dir_y * sin(game->player.rot_velocity);
    game->player.dir_y = old_dir_x * sin(game->player.rot_velocity) + 
                        game->player.dir_y * cos(game->player.rot_velocity);
    
    double old_plane_x = game->player.plane_x;
    game->player.plane_x = game->player.plane_x * cos(game->player.rot_velocity) - 
                          game->player.plane_y * sin(game->player.rot_velocity);
    game->player.plane_y = old_plane_x * sin(game->player.rot_velocity) + 
                          game->player.plane_y * cos(game->player.rot_velocity);
}

void move_player(t_game *game)
{
    update_velocity(&game->player);
    handle_forward_backward(game);
    handle_rotation(game);
}
