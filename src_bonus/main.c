/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 17:40:19 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/09 17:14:21 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"
#include "scene.h"
#include "../include/mlx_rt.h"
#include <stdio.h>

static bool validate_args(int argc, char **argv)
{
	int len;
	if (argc != 2)
	{
		printf("Error: usage %s <scene.rt>\n", argv[0]);
		return (false);
	}
	len = ft_strlen(argv[1]);
	if (len < 4 || ft_strncmp(argv[1] + len - 3, ".rt", 3) != 0)
	{
		printf("Error: file must have .rt extension\n");
		return (false);
	}
	return (true);
}

int	init_mlx(t_minirt *minirt)
{
	minirt->mlx.mlx_ptr = mlx_init(WIDTH, HEIGHT, "miniRt", true);
	if (!minirt->mlx.mlx_ptr)
		return (ft_errorinit(minirt->mlx.mlx_ptr));
	minirt->mlx.img_ptr = mlx_new_image(minirt->mlx.mlx_ptr, WIDTH, HEIGHT);
	if (!minirt->mlx.img_ptr)
		return (ft_errorinit(minirt->mlx.mlx_ptr));
	minirt->mlx.pixels = minirt->mlx.img_ptr->pixels;
	return (0);
}

static void	helper_events(t_minirt *minirt)
{
	mlx_key_hook(minirt->mlx.mlx_ptr, ft_on_keypress, minirt);
	mlx_close_hook(minirt->mlx.mlx_ptr, ft_on_close, minirt);
	mlx_resize_hook(minirt->mlx.mlx_ptr, ft_on_resize, minirt);
}

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (!validate_args(argc, argv))
		return (1);
	ft_memset(&minirt, 0, sizeof(t_minirt));
	minirt.scene = scene_init();
	if (!parse_scene(argv[1], &minirt.scene))
	{
		printf("Failed to parse scene file: %s\n", argv[1]);
		return (1);
	}
	if (init_mlx(&minirt))
		return (1);
	draw(&minirt);
	if (mlx_image_to_window(minirt.mlx.mlx_ptr,	minirt.mlx.img_ptr, 0, 0) == -1)
		return (ft_errorimg(minirt.mlx.mlx_ptr, minirt.mlx.img_ptr));
	helper_events(&minirt);
	mlx_loop(minirt.mlx.mlx_ptr);
	destroy_scene(&minirt.scene);
	mlx_terminate(minirt.mlx.mlx_ptr);
	return (0);
}
