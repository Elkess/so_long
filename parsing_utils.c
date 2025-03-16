/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:31:48 by melkess           #+#    #+#             */
/*   Updated: 2025/03/15 07:55:06 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	fill_map(char *filename, t_game *game, size_t len)
{
	size_t	i;
	char	*holder;

	holder = NULL;
	game->fd = open(filename, O_RDONLY);
	if (game->fd < 0)
		print_err("Failed to open the file !!", game, 1);
	i = 0;
	while (len)
	{
		(free(holder), holder = get_next_line(game->fd));
		game->map[i] = ft_substr(holder, 0, game->win.width);
		game->map2[i] = ft_substr(holder, 0, game->win.width);
		if (!game->map[i] || !game->map2[i])
		{
			game->map[++i] = NULL;
			game->map2[i] = NULL;
			(free(holder), print_err("Map filling failed !!", game, 1));
		}
		i++;
		len--;
	}
	game->map[i] = NULL;
	game->map2[i] = NULL;
	(free(holder), close(game->fd));
}

int	has_valid_char(t_game *game)
{
	size_t	i;
	size_t	j;
	int		chars[256];

	ft_bzero(chars, 256 * sizeof(int));
	i = 1;
	while (game->map[i])
	{
		j = 1;
		while (game->map[i][j])
		{
			if (game->map[i][j] == '0' || game->map[i][j] == '1'
					|| game->map[i][j] == 'C' || game->map[i][j] == 'E'
					|| game->map[i][j] == 'P')
				chars[(int)game->map[i][j]] += 1;
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
	size_t	i;
	size_t	j;

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

void	flood_fill(t_game *game, int x, int y, int *traces)
{
	if (game->map2[x][y] == '1' || game->map2[x][y] == 'a')
		return ;
	if (game->map2[x][y] == '0' || game->map2[x][y] == 'C'
		|| game->map2[x][y] == 'E')
	{
		if (game->map2[x][y] == 'C' || game->map2[x][y] == 'E')
			(*traces)--;
		game->map2[x][y] = 'a';
	}
	flood_fill(game, x +1, y, traces);
	flood_fill(game, x, y +1, traces);
	flood_fill(game, x -1, y, traces);
	flood_fill(game, x, y -1, traces);
}
