/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:28:36 by melkess           #+#    #+#             */
/*   Updated: 2025/03/03 18:02:57 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	is_openable(char *filename, t_game *game)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd > 0)
		game->fd = fd;
	return (fd > 0);
}

static int	is_surrounded_by_walls(t_game *game)
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

static void	map_dimension_helper(char *line, size_t *lenstr, t_game *game)
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

static void	map_dimension(char *filename, t_game *game)
{
	char	*line;
	size_t	lenstr;

	lenstr = 0;
	game->fd = open(filename, O_RDONLY);
	line = get_next_line(game->fd);
	if (!line || !*line)
		print_err("Empty Map !!");
	game->win.width = ft_strlen(line) - (line[ft_strlen(line) -1] == '\n');
	game->win.length = 1;
	map_dimension_helper(line, &lenstr, game);
	if (game->win.width != lenstr)
	{
		game->win.length = 0;
		game->win.width = 0;
		close(game->fd);
		print_err("Map is not a rectangle !");
	}
	game->map = malloc((game->win.length +1) * sizeof(char *));
	game->map2 = malloc((game->win.length +1) * sizeof(char *));
	close(game->fd);
}

void	parsing(char *filename, t_game *game)
{
	t_coordinates	coords;
	int				c;

	if (ft_strcmp(ft_strrchr(filename, '.'), ".ber"))
		print_err("Map's name should have the format *.ber !");
	if (!is_openable(filename, game))
		print_err("Failed to open the file !");
	map_dimension(filename, game);
	fill_map(filename, game);
	if (!is_surrounded_by_walls(game))
		print_err("The map is not surrounded by walls!");
	if (!has_valid_char(game))
		print_err("You did not respect the the 01CEP rules!");
	c = game->collectibles;
	char_position(game->map, 'P', &coords);
	flood_fill(game, (&coords)->x, (&coords)->y, &c);
	char_position(game->map2, 'E', &coords);
	if ((game->map2[(&coords)->x +1][(&coords)->y] != 'a'
		&& game->map2[(&coords)->x][(&coords)->y +1] != 'a'
		&& game->map2[(&coords)->x -1][(&coords)->y] != 'a'
		&& game->map2[(&coords)->x][(&coords)->y -1] != 'a')
		|| c > 0)
		print_err("There is no way to get the exit or all the coins !!");
	check_map_size(game);
}
