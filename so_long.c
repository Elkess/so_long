/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 10:11:20 by melkess           #+#    #+#             */
/*   Updated: 2025/03/09 15:36:56 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	fill_imgs(t_game *game, void *mlx)
{
	t_coordinates	coords;

	game->w_img = mlx_xpm_file_to_image(mlx, "textures/wall.xpm",
			&coords.x, &coords.y);
	game->c_img = mlx_xpm_file_to_image(mlx, "textures/ball.xpm",
			&coords.x, &coords.y);
	game->e_img = mlx_xpm_file_to_image(mlx, "textures/blackhole.xpm",
			&coords.x, &coords.y);
	game->p_img = mlx_xpm_file_to_image(mlx, "textures/player.xpm",
			&coords.x, &coords.y);
	game->s_img = mlx_xpm_file_to_image(mlx, "textures/espace.xpm",
			&coords.x, &coords.y);
	if (!game->c_img || !game->w_img || !game->e_img
		|| !game->p_img || !game->s_img)
		print_err("Failed to load imgs !!", game, 1);
}

static void	put_imgs_to_win(void *mlx, void *mlx_win, t_game *g)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	fill_imgs(g, mlx);
	while (g->map[i])
	{
		j = 0;
		while (g->map[i][j])
		{
			if (g->map[i][j] == '1')
				mlx_put_image_to_window(mlx, mlx_win, g->w_img, j * 64, i * 64);
			if (g->map[i][j] == 'C')
				mlx_put_image_to_window(mlx, mlx_win, g->c_img, j * 64, i * 64);
			if (g->map[i][j] == 'E')
				mlx_put_image_to_window(mlx, mlx_win, g->e_img, j * 64, i * 64);
			if (g->map[i][j] == 'P')
				mlx_put_image_to_window(mlx, mlx_win, g->p_img, j * 64, i * 64);
			if (g->map[i][j] == '0')
				mlx_put_image_to_window(mlx, mlx_win, g->s_img, j * 64, i * 64);
			j++;
		}
		i++;
	}
}

static void	player_moves(int i, int j, t_coordinates cords, t_game *game)
{
	static int		e_trace;
	static size_t	move;
	char			p;

	p = game->map[cords.x + i][cords.y + j];
	if (p == 'C')
		game->collectibles--;
	if (p != '1')
	{
		move++;
		(ft_putnbr(move), write(1, "\n", 1));
		if (p == 'E' && !game->collectibles)
			(write(1, "You Win, Congrats !!!!!\n", 24), destroy(game));
		game->map[cords.x][cords.y] = '0';
		if (e_trace)
		{
			game->map[game->e_cords.x][game->e_cords.y] = 'E';
			e_trace = 0;
		}
		if (p == 'E')
			e_trace = 1;
		game->map[cords.x + i][cords.y + j] = 'P';
		put_imgs_to_win(game->mlxs.mlx, game->mlxs.mlx_win, game);
	}
}

static int	key_press(int keycode, t_game *game)
{
	t_coordinates	cords;
	int				i;
	int				j;

	i = 0;
	j = 0;
	if (keycode == 53)
		destroy(game);
	char_position(game->map, 'P', &cords);
	j = (keycode == 124) * 1 + (keycode == 123) * -1;
	i = (keycode == 125) * 1 + (keycode == 126) * -1;
	player_moves(i, j, cords, game);
	return (1);
}

int	main(int ac, char **av)
{
	t_game	game;

	if (ac == 2 && av[1][0])
	{
		parsing(av[1], &game);
		game.mlxs.mlx = mlx_init();
		if (game.mlxs.mlx == NULL)
			print_err("Failed to establish a mlx connex !!", &game, 1);
		game.mlxs.mlx_win = mlx_new_window(game.mlxs.mlx,
				game.win.width * 64, game.win.length * 64, "so_long");
		if (game.mlxs.mlx_win == NULL)
			print_err("Failed to create the window !!", &game, 1);
		char_position(game.map, 'E', &(game.e_cords));
		put_imgs_to_win(game.mlxs.mlx, game.mlxs.mlx_win, &game);
		mlx_key_hook(game.mlxs.mlx_win, key_press, &game);
		mlx_hook(game.mlxs.mlx_win, 17, 0, destroy, &game);
		mlx_loop(game.mlxs.mlx);
	}
	else
		exit(1);
}
