/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 10:11:20 by melkess           #+#    #+#             */
/*   Updated: 2025/03/03 16:07:25 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	fill_imgs(t_game *game, void *mlx)
{
	t_coordinates	coords;

	if (open("imgs/wall.xpm", O_RDONLY) < 0
		|| open("imgs/espace.xpm", O_RDONLY) < 0
		|| open("imgs/ball1.xpm", O_RDONLY) < 0
		|| open("imgs/blackhole.xpm", O_RDONLY) < 0
		|| open("imgs/player.xpm", O_RDONLY) < 0)
		print_err("Invalid picture path name !!!");
	game->w_img = mlx_xpm_file_to_image(mlx, "imgs/wall.xpm",
			&coords.x, &coords.y);
	game->c_img = mlx_xpm_file_to_image(mlx, "imgs/ball1.xpm",
			&coords.x, & coords.y);
	game->e_img = mlx_xpm_file_to_image(mlx, "imgs/blackhole.xpm",
			&coords.x, & coords.y);
	game->p_img = mlx_xpm_file_to_image(mlx, "imgs/player.xpm",
			&coords.x, & coords.y);
	game->s_img = mlx_xpm_file_to_image(mlx, "imgs/espace.xpm",
			&coords.x, & coords.y);
}

void	put_imgs_to_win(void *mlx, void *mlx_win, t_game *g)
{
	size_t			i;
	size_t			j;

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

void	player_moves(int i, int j, t_coordinates *cords, t_game *game)
{
	char	p;

	p = game->map[cords->x + i][cords->y + j];
	if (p == 'C')
		game->collectibles--;
	if ((p != '1' && p != 'E') || (p == 'E' && !game->collectibles))
	{
		if (p == 'E')
		{
			write(1, "You Win, Congrats !!!!!\n", 24);
			mlx_destroy_window(game->mlxs.mlx, game->mlxs.mlx_win);
			exit(0);
		}
		game->map[cords->x][cords->y] = '0';
		game->map[cords->x + i][cords->y + j] = 'P';
		put_imgs_to_win(game->mlxs.mlx, game->mlxs.mlx_win, game);
	}
}

int	key_press(int keycode, t_game *game)
{
	t_coordinates	*cords;
	int				i;
	int				j;

	i = 0;
	j = 0;
	cords = malloc(sizeof(cords));
	char_position(game->map, 'P', cords);
	j = (keycode == 124) * 1 + (keycode == 123) * -1;
	i = (keycode == 125) * 1 + (keycode == 126) * -1;
	player_moves(i, j, cords, game);
	if (keycode == 53)
	{
		mlx_destroy_window(game->mlxs.mlx, game->mlxs.mlx_win);
		exit(0);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_game	*game;

	if (ac == 2 && av[1][0])
	{
		game = malloc(sizeof(t_game));
		parsing(av[1], game);
		game->mlxs.mlx = mlx_init();
		game->mlxs.mlx_win = mlx_new_window(game->mlxs.mlx,
				game->win.width * 64, game->win.length * 64, "so_long");
		put_imgs_to_win(game->mlxs.mlx, game->mlxs.mlx_win, game);
		mlx_key_hook(game->mlxs.mlx_win, key_press, game);
		mlx_loop(game->mlxs.mlx);
	}
	else
		exit(1);
	exit(0);
}
