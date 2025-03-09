/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 09:57:56 by melkess           #+#    #+#             */
/*   Updated: 2025/03/09 15:34:11 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <mlx.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

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

typedef struct s_coordinates
{
	int	x;
	int	y;
}	t_coordinates;

typedef struct s_game
{
	t_windows		win;
	t_mlx			mlxs;
	char			**map2;
	char			**map;
	t_coordinates	e_cords;
	void			*p_img;
	void			*e_img;
	void			*c_img;
	void			*w_img;
	void			*s_img;
	int				collectibles;
	int				fd;
}	t_game;

char	*get_next_line(int fd);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
char	*ft_strdup( char *s1);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_substr(char const *s, size_t start, size_t len);
void	free_map(char **map);
void	print_err(char	*s, t_game *game, int flag);
char	*ft_strrchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
void	ft_bzero(void *s, size_t n);
void	parsing(char *filename, t_game *game);
void	fill_map(char *filename, t_game *game);
int		has_valid_char(t_game *game);
void	char_position(char **map, char c, t_coordinates *coords);
void	flood_fill(t_game *game, int x, int y, int *c);
void	check_map_size(t_game *game);
int		destroy(t_game *game);
void	ft_putnbr(size_t n);

#endif