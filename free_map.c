/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:03:43 by melkess           #+#    #+#             */
/*   Updated: 2025/03/10 09:42:30 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	free_map(char **map)
{
	size_t	i;

	i = 0;
	while (map && map[i])
		free(map[i++]);
	free(map);
}

void	destroyer(t_game *game)
{
	free_map(game->map);
	free_map(game->map2);
	if (game->c_img)
		mlx_destroy_image(game->mlxs.mlx, game->c_img);
	if (game->w_img)
		mlx_destroy_image(game->mlxs.mlx, game->w_img);
	if (game->e_img)
		mlx_destroy_image(game->mlxs.mlx, game->e_img);
	if (game->s_img)
		mlx_destroy_image(game->mlxs.mlx, game->s_img);
	if (game->p_img)
		mlx_destroy_image(game->mlxs.mlx, game->p_img);
	mlx_destroy_window(game->mlxs.mlx, game->mlxs.mlx_win);
}
