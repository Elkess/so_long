/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 09:57:56 by melkess           #+#    #+#             */
/*   Updated: 2025/02/27 17:23:21 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

#include <libc.h>
# include <mlx.h>
# include <limits.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include "get_next_line/get_next_line.h"

typedef struct s_game
{
	int		fd;
	char	**map;
	char	**map2;
	size_t	width;
	size_t	length;
	size_t	char_counter;
}	t_game;

typedef struct s_coordinates
{
	int	x;
	int	y;
}	t_coordinates;


char	*ft_strrchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
void	ft_bzero(void *s, size_t n);
void	ft_exit(int n);
void	*ft_malloc(size_t	size);

#endif