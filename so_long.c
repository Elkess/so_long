/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 10:11:20 by melkess           #+#    #+#             */
/*   Updated: 2025/03/02 22:30:52 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	print_err(char	*s)
{
	write(2, "Error\n", 6);
	write(2, s, ft_strlen(s));
	exit(1);
}

int	is_surrounded_by_walls(t_game *game)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (game->map[i][j])
	{
		if (game->map[i][j++] != '1')
			return (0);
	}
	i = game->win.length -1;
	j = 0;
	while (game->map[i][j])
	{
		if (game->map[i][j++] != '1')
			return (0);
	}
	i = 1;
	j = game->win.width -1;
	while (game->map[i] && game->map[i][0] == '1' && game->map[i][j] == '1' )
		i++;
	if (game->win.length != i)
		return (0);
	return (1);
}

int	is_openable(char *filename, t_game *game)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd > 0)
		game->fd = fd;
	return (fd > 0);
}

void	map_dimension_helper(char *line, size_t *lenstr, t_game *game)
{
	while (line)
	{
		free(line);
		line = get_next_line(game->fd);
		if (!line)
			break ;
		*lenstr = ft_strlen(line) - (line[ft_strlen(line) - 1] == '\n');
		game->win.length++;
		if (game->win.width != *lenstr)
			break ;
	}
	free(line);
}

void	map_dimension(char *filename, t_game *game)
{
	char	*line;
	size_t	lenstr;

	lenstr = 0;
	game->fd = open(filename, O_RDONLY);
	line = get_next_line(game->fd);
	if (!line)
		return ;
	game->win.width = ft_strlen(line) - (line[ft_strlen(line) -1] == '\n');
	game->win.length = 1;
	map_dimension_helper(line, &lenstr, game);
	if (game->win.width != lenstr)
	{
		game->win.length = 0;
		game->win.width = 0;
		close(game->fd);
		return ;
	}
	game->map = malloc((game->win.length +1) * sizeof(char *));
	game->map2 = malloc((game->win.length +1) * sizeof(char *));
	close(game->fd);
}

void	fill_map(char *filename, t_game *game)
{
	size_t	i;
	size_t	len;
	char	*holder;

	game->fd = open(filename, O_RDONLY);
	i = 0;
	len = game->win.length;
	while (len)
	{
		holder = get_next_line(game->fd);
		game->map[i] = ft_substr(holder, 0, game->win.width);
		game->map2[i] = ft_substr(holder, 0, game->win.width);
		i++;
		len--;
	}
	game->map[i] = NULL;
	game->map2[i] = NULL;
	close(game->fd);
}

int	has_valid_char(t_game *game)
{
	size_t	i;
	size_t	j;
	int		chars[256];

	ft_bzero(chars, 256);
	i = 1;
	while (game->map[i])
	{
		j = 1;
		while (game->map[i][j])
		{
			if (game->map[i][j] == '0' || game->map[i][j] == '1'
			|| game->map[i][j] == 'C' || game->map[i][j] == 'E' || game->map[i][j] == 'P')
				chars[game->map[i][j]] += 1;
			else
				return (0);
			j++;
		}
		i++;
	}
	if (chars['C'] == 0 || chars['E'] != 1 || chars['P'] != 1)
			return (0); 
	game->collectibles = chars['C'];
	return (1);
}

void	char_position(char **map, char c, t_coordinates *coords)
{
	size_t			i;
	size_t			j;

	coords->x = 0;
	coords->y = 0;
	i = 0;
	j = 0;

	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == c)
			{
				coords->x = i;
				coords->y = j;
			}
			j++;
		}
		i++;
	}
}

void	flood_fill(t_game *game, int x, int y)
{
	if (game->map2[x][y] == '1' || game->map2[x][y] == 'a')
		return ;
	if (game->map2[x][y] == '0' || game->map2[x][y] == 'C')
	{
		if (game->map2[x][y] == 'C')
			game->collectibles--;
		game->map2[x][y] = 'a';	
	}
	flood_fill(game, x +1, y);
	flood_fill(game, x, y +1);
	flood_fill(game, x -1, y);
	flood_fill(game, x, y -1);
}

// void	check_map_size(t_game *game)
// {
	
// }

void	parsing(char *filename, t_game *game)
{
	t_coordinates	*coords;

	coords = malloc(sizeof(t_coordinates));
	if (ft_strcmp(ft_strrchr(filename, '.'), ".ber"))
		print_err("Map's name should have the format *.ber !");
	if (!is_openable(filename, game))
		print_err("Failed to open the file !");
	map_dimension(filename, game);
	if (!(game->win.length * game->win.width))
		print_err("Invalid Map !");
	fill_map(filename, game);
	if (!is_surrounded_by_walls(game))
		print_err("The map is not surrounded by walls!");
	if (!has_valid_char(game))
		print_err("You did not respect the the 01CEP rules!");
	char_position(game->map, 'P', coords);
	flood_fill(game, coords->x, coords->y);
	char_position(game->map2, 'E', coords);
	if ((game->map2[coords->x +1][coords->y] != 'a'
		&& game->map2[coords->x][coords->y +1] != 'a'
		&& game->map2[coords->x -1][coords->y] != 'a'
		&& game->map2[coords->x][coords->y -1] != 'a')
		|| game->collectibles > 0)
		print_err("Ther is no way to get the exit or all the coins !!");
	// check_map_size(game);
}

void	fill_imgs(t_game *game, void *mlx)
{
	t_coordinates	coords;
	char			*s;

	if (open("textures/wall.xpm", O_RDONLY) < 0
		|| open("textures/espace.xpm", O_RDONLY) < 0
		|| open("textures/ball1.xpm", O_RDONLY) < 0
		|| open("textures/blackhole.xpm", O_RDONLY) < 0
		|| open("textures/player.xpm", O_RDONLY) < 0)
		print_err("Invalid picture path name !!!");
	game->w_img = mlx_xpm_file_to_image(mlx, "textures/wall.xpm", &coords.x, &coords.y);
	game->c_img = mlx_xpm_file_to_image(mlx, "textures/ball1.xpm", &coords.x, & coords.y);
	game->e_img = mlx_xpm_file_to_image(mlx, "textures/blackhole.xpm", &coords.x, & coords.y);
	game->p_img = mlx_xpm_file_to_image(mlx, "textures/player.xpm", &coords.x, & coords.y);
	game->s_img = mlx_xpm_file_to_image(mlx, "textures/espace.xpm", &coords.x, & coords.y);
}

void	put_imgs_to_win(void *mlx, void *mlx_win, t_game *game)
{
	size_t			i;
	size_t			j;

	i = 0;
	j = 0;
	fill_imgs(game, mlx);
	while (game->map[i])
	{
		j = 0;
		while (game->map[i][j])
		{
			if (game->map[i][j] == '1')
				mlx_put_image_to_window(mlx, mlx_win, game->w_img, j * 64, i * 64);
			if (game->map[i][j] == 'C')
				mlx_put_image_to_window(mlx, mlx_win, game->c_img, j * 64, i * 64);
			if (game->map[i][j] == 'E')
				mlx_put_image_to_window(mlx, mlx_win, game->e_img, j * 64, i * 64);
			if (game->map[i][j] == 'P')
				mlx_put_image_to_window(mlx, mlx_win, game->p_img, j * 64, i * 64);
			if (game->map[i][j] == '0')
				mlx_put_image_to_window(mlx, mlx_win, game->s_img, j * 64, i * 64);
			j++;
		}
		i++;
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
	// // if (
	// 	(, game->win.width);
	// 	// || 
	// 	game->win.length - cords->x > 1
	// 	// )
	// // {
		game->map[cords->x][cords->y] = '0';
		game->map[cords->x + i][cords->y + j] = 'P';
	// }
	put_imgs_to_win(game->mlxs.mlx, game->mlxs.mlx_win, game);
	if (keycode == 53)
	{
		mlx_destroy_window(game->mlxs.mlx, game->mlxs.mlx_win);
		exit(0);
	}
	return(1);
}

int	main(int ac, char **av)
{
	t_game	*game;


	if (ac == 2 && av[1][0])
	{
		game = malloc(sizeof(t_game));
		parsing(av[1], game);
		game->mlxs.mlx = mlx_init();
		game->mlxs.mlx_win = mlx_new_window(game->mlxs.mlx, game->win.width * 64, game->win.length * 64, "so_long");
		put_imgs_to_win(game->mlxs.mlx, game->mlxs.mlx_win, game);
		mlx_key_hook(game->mlxs.mlx_win, key_press, game);
		mlx_loop(game->mlxs.mlx);
	}
	else
		exit(1);
	exit(0);
}
