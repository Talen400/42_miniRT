/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 01:44:49 by tlavared          #+#    #+#             */
/*   Updated: 2026/01/31 02:08:29 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRt.h"
#include "../include/mlx_rt.h"

static inline void	put(uint8_t *pixels, int x, int y,
		uint32_t color)
{
	int	index;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		index = (y * WIDTH + x) * 4;
		pixels[index + 0] = (color >> 24) & 0xFF;
		pixels[index + 1] = (color >> 16) & 0xFF;
		pixels[index + 2] = (color >> 8) & 0xFF;
		pixels[index + 3] = color & 0xFF;
	}
}

static void	drawing(t_minirt *minirt)
{
	uint32_t	color;
	int			i;
	int			j;

	color = 0xFF0000FF;
	i = 0;
	j = 0;
	while (i < WIDTH)
	{
		j = 0;
		while (j < HEIGHT)
		{
			put(minirt->mlx.pixels, i, j, color);
			j++;
		}
		i++;
	}
}

int	draw(t_minirt *minirt)
{
	ft_clearimg(minirt->mlx.pixels);
	drawing(minirt);
	return (0);
}
