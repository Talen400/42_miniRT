/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 17:40:19 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/02 13:20:22 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"
#include "scene.h"
#include "../include/mlx_rt.h"
#include <stdio.h>

int	ft_init(t_minirt *minirt)
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

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (argc != 2)
	{
		printf("Usage: %s <scene.rt>\n", argv[0]);
		return (1);
	}
	ft_memset(&minirt, 0, sizeof(t_minirt));
	minirt.scene = scene_init();
	if (!parse_scene(argv[1], &minirt.scene))
	{
		printf("Failed to parse scene file: %s\n", argv[1]);
		return (1);
	}
	    init_camera(&minirt.scene.camera);
    if (ft_init(&minirt))
        return (1);
    render_scene(&minirt);
    mlx_image_to_window(minirt.mlx.mlx_ptr, minirt.mlx.img_ptr, 0, 0);
    mlx_loop(minirt.mlx.mlx_ptr);
    mlx_terminate(minirt.mlx.mlx_ptr);
    //free_scene(&minirt.scene);
	return (0);
}
