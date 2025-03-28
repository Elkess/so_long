/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:44:22 by melkess           #+#    #+#             */
/*   Updated: 2025/03/15 08:05:21 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i--;
	}
	if ((char) c == '\0')
		return ((char *)(s + i));
	return (NULL);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	if (!s1)
		return (1);
	i = 0;
	while ((s1[i] || s2[i]) && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*b;
	size_t			i;

	b = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		b[i] = 0;
		i++;
	}
}

void	print_err(char	*s, t_game *game, int flag)
{
	size_t	i;

	i = 0;
	write(2, "Error\n", 6);
	write(2, s, ft_strlen(s));
	write(2, "\n", 1);
	close(game->fd);
	if (flag)
	{
		while ((game->map2 && game->map2[i]) || (game->map && game->map[i]))
		{
			if (game->map && game->map[i])
				free(game->map[i]);
			if (game->map2 && game->map2[i])
				free(game->map2[i]);
			i++;
		}
		free(game->map);
		free(game->map2);
	}
	exit(1);
}

int	destroy(t_game *game)
{
	destroyer(game);
	exit(0);
}
