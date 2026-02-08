/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 01:01:01 by tlavared          #+#    #+#             */
/*   Updated: 2026/01/31 02:07:05 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	ft_errorinit(mlx_t *mlx)
{
	if (mlx)
		mlx_terminate(mlx);
	printf("%s", mlx_strerror(mlx_errno));
	return (1);
}

int	ft_errorimg(mlx_t *mlx, mlx_image_t *img)
{
	if (img)
		mlx_delete_image(mlx, img);
	mlx_terminate(mlx);
	return (1);
}

void	ft_clearimg(uint8_t *pixels)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			put(pixels, x, y, 0x000000FF);
			x++;
		}
		y++;
	}
}
