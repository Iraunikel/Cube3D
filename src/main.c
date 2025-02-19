/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iunikel <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:44:44 by iunikel           #+#    #+#             */
/*   Updated: 2025/02/19 15:37:12 by iunikel          ###   ########.fr       */
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

static int	close_window(t_game *game)
{
	cleanup_game(game);
	exit(0);
	return (0);
}

static int	validate_args(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_putstr_fd("Error\nUsage: ./cub3D <map.cub>\n", 2);
		return (0);
	}
	if (!validate_file_extension(argv[1]))
	{
		ft_putstr_fd("Error\nInvalid file extension. Must be .cub\n", 2);
		return (0);
	}
	return (1);
}

static int	setup_game(t_game *game, char *map_file)
{
	if (!init_game(game))
	{
		ft_putstr_fd("Error\nFailed to initialize game\n", 2);
		return (0);
	}
	printf("Game initialized successfully. Parsing map file...\n");
	if (!parse_map_file(map_file, game))
	{
		cleanup_game(game);
		return (0);
	}
	return (1);
}

static void	setup_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, key_press, game);
	mlx_hook(game->win, 3, 1L << 1, key_release, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_loop_hook(game->mlx, game_loop, game);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (!validate_args(argc, argv))
		return (1);
	if (!setup_game(&game, argv[1]))
		return (1);
	printf("Map file parsed successfully. Setting up player...\n");
	init_player_direction(&game,
		game.map.map[(int)game.player.y][(int)game.player.x]);
	setup_hooks(&game);
	printf("Starting game loop...\n");
	mlx_loop(game.mlx);
	cleanup_game(&game);
	return (0);
}
