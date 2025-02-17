/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:44:44 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/17 22:44:52 by iunikel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

static void	init_player_direction(t_game *game, char direction)
{
	game->player.dir_x = 0;
	game->player.dir_y = 0;
	game->player.plane_x = 0;
	game->player.plane_y = 0;
	if (direction == 'N')
	{
		game->player.dir_y = -1;
		game->player.plane_x = 0.66;
	}
	else if (direction == 'S')
	{
		game->player.dir_y = 1;
		game->player.plane_x = -0.66;
	}
	else if (direction == 'E')
	{
		game->player.dir_x = 1;
		game->player.plane_y = 0.66;
	}
	else if (direction == 'W')
	{
		game->player.dir_x = -1;
		game->player.plane_y = -0.66;
	}
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
	{
		ft_putstr_fd("Error\nUsage: ./cub3D <map.cub>\n", 2);
		return (1);
	}
	if (!validate_file_extension(argv[1]))
	{
		ft_putstr_fd("Error\nInvalid file extension. Must be .cub\n", 2);
		return (1);
	}
	if (!init_game(&game))
	{
		ft_putstr_fd("Error\nFailed to initialize game\n", 2);
		return (1);
	}
	printf("Game initialized successfully. Parsing map file...\n");
	if (!parse_map_file(argv[1], &game))
	{
		cleanup_game(&game);
		return (1);
	}
	printf("Map file parsed successfully. Setting up player...\n");
	// Initialize player direction based on map character
	init_player_direction(&game,
		game.map.map[(int)game.player.y][(int)game.player.x]);
	// Set up hooks
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	printf("Starting game loop...\n");
	mlx_loop(game.mlx);
	cleanup_game(&game);
	return (0);
}
