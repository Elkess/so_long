/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 10:11:20 by melkess           #+#    #+#             */
/*   Updated: 2025/02/27 17:26:37 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	print_err(char	*s)
{
	write(2, "Error\n", 6);
	write(2, s, ft_strlen(s));
	ft_exit(1);
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
	i = game->length -1;
	j = 0;
	while (game->map[i][j])
	{
		if (game->map[i][j++] != '1')
			return (0);
	}
	i = 1;
	j = game->width -1;
	while (game->map[i] && game->map[i][0] == '1' && game->map[i][j] == '1' )
		i++;
	if (game->length != i)
		return (0);
	return (1);
}

int	is_openable(char *filename, t_game *game)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		close(fd);
		return (0);
	}
	game->fd = fd;
	return (1);
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
		game->length++;
		if (game->width != *lenstr)
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
	game->width = ft_strlen(line) - (line[ft_strlen(line) -1] == '\n');
	game->length = 1;
	map_dimension_helper(line, &lenstr, game);
	if (game->width != lenstr)
	{
		game->length = 0;
		game->width = 0;
		close(game->fd);
		return ;
	}
	game->map = ft_malloc((game->length +1) * sizeof(char *));
	game->map2 = ft_malloc((game->length +1) * sizeof(char *));
	close(game->fd);
}

void	fill_map(char *filename, t_game *game)
{
	size_t	i;
	size_t	len;
	char	*holder;

	game->fd = open(filename, O_RDONLY);
	i = 0;
	len = game->length;
	while (len)
	{
		holder = get_next_line(game->fd);
		game->map[i] = ft_substr(holder, 0, game->width);
		game->map2[i] = ft_substr(holder, 0, game->width);
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
	return (1);
}

void	char_position(char **map, char c, t_coordinates *coords)
{
	size_t			i;
	size_t			j;

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

void	flood_fill(char **map, int x, int y)
{
	if (map[x][y] == '1' || map[x][y] == 'c')
		return ;
	if (map[x][y] == '0' || map[x][y] == 'C')
		map[x][y] = 'c';
	flood_fill(map, x +1, y);
	flood_fill(map, x, y +1);
	flood_fill(map, x -1, y);
	flood_fill(map, x, y -1);
}


void	parsing(char *filename, t_game *game)
{
	t_coordinates	*coords;

	coords = ft_malloc(sizeof(t_coordinates));
	coords->x = 0;
	coords->y = 0;
	if (ft_strcmp(ft_strrchr(filename, '.'), ".ber"))
		print_err("Map's name should have the format *.ber !");
	if (!is_openable(filename, game))
		print_err("Failed to open the file !");
	map_dimension(filename, game);
	if (!(game->length * game->width))
		print_err("Invalid Map !");
	fill_map(filename, game);
	if (!is_surrounded_by_walls(game))
		print_err("The map is not surrounded by walls!");
	if (!has_valid_char(game))
		print_err("You did not respect the the 01CEP rules!");
	char_position(game->map2, 'E', coords);
	printf("x: %d; y: %d\n", coords->x, coords->y);
	flood_fill(game->map2, coords->x, coords->y);
	size_t i = 0;
	while (game->map2[i])
	{
		puts(game->map2[i]);
		i++;
	}
	// if (!player_has_valid_path(game->map))
	// 	print_err("Player can not reach a Collectible or an Exit !!!");
}

int	main(int ac, char **av)
{
	t_game	*game;

	if (ac == 2)
	{
		game = ft_malloc(sizeof(t_game));
		parsing(av[1], game);
	}
	else
		ft_exit(1);
	ft_exit(0);
}
