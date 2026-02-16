/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-11 18:39:36 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-11 18:39:36 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"
#include "scene.h"
#include "../include/mlx_rt.h"

void	ft_on_keypress(mlx_key_data_t keydata, void *param)
{
	t_minirt	*minirt;

	minirt = (t_minirt *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(minirt->mlx.mlx_ptr);
}

void	ft_on_close(void *param)
{
	t_minirt	*minirt;

	minirt = (t_minirt *)param;
	mlx_close_window(minirt->mlx.mlx_ptr);
}

void	ft_on_resize(int32_t width, int32_t height, void *param)
{
	t_minirt	*minirt;

	minirt = (t_minirt *)param;
	minirt->scene.width = width;
	minirt->scene.height = height;
	mlx_resize_image(minirt->mlx.img_ptr, width, height);
	minirt->mlx.pixels = minirt->mlx.img_ptr->pixels;
	calculate_viewport(&minirt->scene.camera, width, height);
	draw(minirt);
}
