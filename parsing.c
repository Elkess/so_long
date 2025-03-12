/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:28:36 by melkess           #+#    #+#             */
/*   Updated: 2025/03/12 17:13:32 by melkess          ###   ########.fr       */
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
		if (game->win.length > 22)
		{
			free(line);
			print_err("you have exceeded the boundries !!", game, 0);
		}
		if (game->win.width != *lenstr)
			break ;
	}
	free(line);
	close(game->fd);
}

static void	map_dimension(t_game *game)
{
	char	*line;
	size_t	lenstr;

	lenstr = 0;
	line = get_next_line(game->fd);
	if (!line)
		print_err("Empty Map !!", game, 0);
	game->win.width = ft_strlen(line) - (line[ft_strlen(line) -1] == '\n');
	if (game->win.width > 40)
		(free(line), print_err("you have exceeded the boundries !!", game, 0));
	game->win.length = 1;
	map_dimension_helper(line, &lenstr, game);
	if (game->win.width != lenstr)
	{
		game->win.length = 0;
		game->win.width = 0;
		print_err("Map is not a rectangle !", game, 0);
	}
	game->map = malloc((game->win.length +1) * sizeof(char *));
	game->map2 = malloc((game->win.length +1) * sizeof(char *));
	if (!game->map2 || !game->map)
		print_err("Map allocation failed !!", game, 1);
}

void	parsing(char *filename, t_game *game)
{
	t_coordinates	coords;
	int				traces;

	if (ft_strcmp(ft_strrchr(filename, '.'), ".ber"))
		print_err("Map's name should have the format *.ber !", game, 0);
	if (!is_openable(filename, game))
		print_err("Failed to open the file !", game, 0);
	map_dimension(game);
	fill_map(filename, game, game->win.length);
	if (!is_surrounded_by_walls(game))
		print_err("The map is not surrounded by walls!", game, 1);
	if (!has_valid_char(game))
		print_err("You did not respect the the 01CEP rules!", game, 1);
	traces = game->collectibles +1;
	char_position(game->map, 'P', &coords);
	flood_fill(game, coords.x, coords.y, &traces);
	if (traces > 0)
		print_err("There isn't a way for all the coins or the exit !", game, 1);
}
