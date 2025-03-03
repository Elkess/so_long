/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 09:57:56 by melkess           #+#    #+#             */
/*   Updated: 2025/03/03 20:59:59 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <mlx.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "get_next_line/get_next_line.h"

typedef struct s_windows
{
	size_t	width;
	size_t	length;
}	t_windows;

typedef struct s_mlx
{
	void	*mlx;
	void	*mlx_win;
}	t_mlx;

typedef struct s_game
{
	t_windows	win;
	t_mlx		mlxs;
	char		**map2;
	char		**map;
	void		*p_img;
	void		*e_img;
	void		*c_img;
	void		*w_img;
	void		*s_img;
	int			collectibles;
	int			fd;
}	t_game;

typedef struct s_coordinates
{
	int	x;
	int	y;
}	t_coordinates;

char	*ft_strrchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
void	ft_bzero(void *s, size_t n);
void	parsing(char *filename, t_game *game);
void	print_err(char	*s);
void	fill_map(char *filename, t_game *game);
int		has_valid_char(t_game *game);
void	char_position(char **map, char c, t_coordinates *coords);
void	flood_fill(t_game *game, int x, int y, int *c);
void	check_map_size(t_game *game);
int		destroy(t_game *game);
void	ft_putnbr(size_t n);

#endif