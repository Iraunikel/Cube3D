/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:43:24 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/19 15:41:02 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	key_press(int keycode, t_game *game)
{
    if (keycode == KEY_ESC)
    {
        cleanup_game(game);
        exit(0);
    }
    if (keycode == KEY_W || keycode == KEY_UP)
        game->player.move_w = 1;
    if (keycode == KEY_S || keycode == KEY_DOWN)
        game->player.move_s = 1;
    if (keycode == KEY_A)
        game->player.move_a = 1;
    if (keycode == KEY_D)
        game->player.move_d = 1;
    if (keycode == KEY_LEFT)
        game->player.rot_left = 1;
    if (keycode == KEY_RIGHT)
        game->player.rot_right = 1;
    return (0);
}

int	key_release(int keycode, t_game *game)
{
    if (keycode == KEY_W || keycode == KEY_UP)
        game->player.move_w = 0;
    if (keycode == KEY_S || keycode == KEY_DOWN)
        game->player.move_s = 0;
    if (keycode == KEY_A)
        game->player.move_a = 0;
    if (keycode == KEY_D)
        game->player.move_d = 0;
    if (keycode == KEY_LEFT)
        game->player.rot_left = 0;
    if (keycode == KEY_RIGHT)
        game->player.rot_right = 0;
    return (0);
} 
