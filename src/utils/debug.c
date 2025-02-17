/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:16:18 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/17 20:16:20 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static void print_color(const char *name, t_color color)
{
    printf("%s color: R(%d) G(%d) B(%d)\n", name, color.r, color.g, color.b);
}

static void print_map(t_map *map)
{
    int i;

    printf("\nMap dimensions: %dx%d\n", map->width, map->height);
    printf("Map content:\n");
    i = 0;
    while (i < map->height)
    {
        printf("%s\n", map->map[i]);
        i++;
    }
}

void    print_game_state(t_game *game)
{
    printf("\n=== GAME STATE DEBUG INFO ===\n");
    printf("\nTexture paths:\n");
    printf("North: %s\n", game->north_texture);
    printf("South: %s\n", game->south_texture);
    printf("West: %s\n", game->west_texture);
    printf("East: %s\n", game->east_texture);
    
    printf("\nColors:\n");
    print_color("Ceiling", game->ceiling_color);
    print_color("Floor", game->floor_color);
    
    printf("\nPlayer position: (%.2f, %.2f)\n", game->player.x, game->player.y);
    
    print_map(&game->map);
    printf("\n=== END DEBUG INFO ===\n\n");
} 