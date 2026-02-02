/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 17:40:19 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/02 18:43:22 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include <stdio.h>
#include "../include/miniRt.h"
#include "../include/mlx_rt.h"

t_camera	init_camera(void)
{
	t_camera camera;
	t_vec3	tmp1;
	t_vec3	tmp2;

	camera.fov = 1.0;
	camera.viewport_height = 2.0;
	camera.viewport_width = camera.viewport_height * ((double) WIDTH / (double) HEIGHT);
	camera.position = vec3_create(0, 0, 0);
	camera.horizontal = vec3_create(camera.viewport_width, 0.0, 0.0);
	camera.vertical = vec3_create(0.0, -camera.viewport_height, 0.0);
	// P - CH/2 - CV/2 - FOV
	tmp1 = vec3_subtract(camera.position, vec3_divide(camera.horizontal, 2));
	tmp2 = vec3_subtract(tmp1, vec3_divide(camera.vertical, 2));
	camera.lower_left_corner = vec3_subtract(tmp2, vec3_create(0.0, 0.0, camera.fov));
	return (camera);
}

int	init_scene(t_minirt *minirt)
{
	minirt->scene.camera = init_camera();
	return (0);
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

int main(int argc, char **argv)
{
	t_minirt	minirt;

	/*
	if (argc != 2)
	{
		printf("Usage: %s <scene.rt>\n", argv[0]);
		return (1);
	}
	printf("Parsing scene file: %s\n", argv[1]);
	*/
	if (argc != 1)
		return (1);
	ft_memset(&minirt, 0, sizeof(t_minirt ));
	if (init_mlx(&minirt))
		return (1);
	if (init_scene(&minirt))
		return (1);
	draw(&minirt);
	if (mlx_image_to_window(minirt.mlx.mlx_ptr,
			minirt.mlx.img_ptr, 0, 0) == -1)
		return (ft_errorimg(minirt.mlx.mlx_ptr, minirt.mlx.img_ptr));
	mlx_loop(minirt.mlx.mlx_ptr);
	mlx_terminate(minirt.mlx.mlx_ptr);
	(void ) argv;
	return (0);
}
