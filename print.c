/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:57:25 by melkess           #+#    #+#             */
/*   Updated: 2025/03/10 09:55:45 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(size_t n)
{
	size_t	arr[20];
	size_t	i;

	i = 0;
	while (n)
	{
		arr[i] = n % 10;
		n /= 10;
		i++;
	}
	while (i)
		ft_putchar(arr[--i] +48);
}
