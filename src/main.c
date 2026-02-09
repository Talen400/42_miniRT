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
#include "parser.h"
#include "scene.h"
#include "../include/mlx_rt.h"
#include <stdio.h>

static void	print_objects(t_object *obj)
{
	int	i;

	i = 0;
	while (obj)
	{
		i++;
		if (obj->type == SPHERE)
			printf("  [%d] SPHERE: center(%.1f,%.1f,%.1f) d=%.1f "
				"color(%.0d,%.0d,%.0d)\n", i,
				obj->shape.sphere.center.x, obj->shape.sphere.center.y,
				obj->shape.sphere.center.z, obj->shape.sphere.diameter,
				obj->shape.sphere.color.r, obj->shape.sphere.color.g,
				obj->shape.sphere.color.b);
		else if (obj->type == PLANE)
			printf("  [%d] PLANE: point(%.1f,%.1f,%.1f) "
				"normal(%.1f,%.1f,%.1f) color(%.0d,%.0d,%.0d)\n", i,
				obj->shape.plane.point.x, obj->shape.plane.point.y,
				obj->shape.plane.point.z, obj->shape.plane.normal.x,
				obj->shape.plane.normal.y, obj->shape.plane.normal.z,
				obj->shape.plane.color.r, obj->shape.plane.color.g,
				obj->shape.plane.color.b);
		else if (obj->type == CYLINDER)
			printf("  [%d] CYLINDER: center(%.1f,%.1f,%.1f) "
				"d=%.1f h=%.1f color(%.0d,%.0d,%.0d)\n", i,
				obj->shape.cylinder.center.x,
				obj->shape.cylinder.center.y,
				obj->shape.cylinder.center.z,
				obj->shape.cylinder.diameter,
				obj->shape.cylinder.height,
				obj->shape.cylinder.color.r,
				obj->shape.cylinder.color.g,
				obj->shape.cylinder.color.b);
		obj = obj->next;
	}
}

static void	print_scene(t_scene *scene)
{
	t_light	*light;
	int		i;

	printf("=== SCENE PARSE TEST ===\n\n");
	printf("Ambient: intensity=%.1f color(%.0d,%.0d,%.0d)\n",
		scene->ambient.intensity,
		scene->ambient.color.r, scene->ambient.color.g,
		scene->ambient.color.b);
	printf("Camera: pos(%.1f,%.1f,%.1f) fov=%.1f\n",
		scene->camera.position.x, scene->camera.position.y,
		scene->camera.position.z, scene->camera.fov);
	light = scene->lights;
	i = 0;
	while (light)
	{
		printf("Light[%d]: pos(%.1f,%.1f,%.1f) brightness=%.1f "
			"color(%.0dd,%.0d,%.0d)\n", ++i,
			light->position.x, light->position.y, light->position.z,
			light->brightness,
			light->color.r, light->color.g, light->color.b);
		light = light->next;
	}
	printf("Objects:\n");
	print_objects(scene->objects);
	printf("\n=== END ===\n");
}

t_camera	init_camera(void)
{
	t_camera camera;
	t_vec3	tmp1;
	t_vec3	tmp2;

	camera.fov = 1.0;
	// Calculo da proporção da câmera
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
	}
	(void ) argv;
	print_scene(&minirt.scene);
	return (0);
}
