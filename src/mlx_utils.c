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

void	put(t_minirt *minirt, int x, int y, uint32_t color)
{
	int	index;
	int	width;
	int	height;

	width = minirt->scene.width;
	height = minirt->scene.height;
	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		index = (y * width + x) * 4;
		minirt->mlx.pixels[index + 0] = (color >> 24) & 0xFF;
		minirt->mlx.pixels[index + 1] = (color >> 16) & 0xFF;
		minirt->mlx.pixels[index + 2] = (color >> 8) & 0xFF;
		minirt->mlx.pixels[index + 3] = color & 0xFF;
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

void	ft_clearimg(t_minirt *minirt)
{
	int	x;
	int	y;

	y = 0;
	while (y < minirt->scene.height)
	{
		x = 0;
		while (x < minirt->scene.width)
		{
			put(minirt, x, y, 0x000000FF);
			x++;
		}
		y++;
	}
}
