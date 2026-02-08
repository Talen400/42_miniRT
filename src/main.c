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

static void	print_objects(t_object *obj)
{
	int	i;

	i = 0;
	while (obj)
	{
		i++;
		if (obj->type == SPHERE)
			printf("  [%d] SPHERE: center(%.1f,%.1f,%.1f) d=%.1f "
				"color(%.0f,%.0f,%.0f)\n", i,
				obj->shape.sphere.center.x, obj->shape.sphere.center.y,
				obj->shape.sphere.center.z, obj->shape.sphere.diameter,
				obj->shape.sphere.color.x, obj->shape.sphere.color.y,
				obj->shape.sphere.color.z);
		else if (obj->type == PLANE)
			printf("  [%d] PLANE: point(%.1f,%.1f,%.1f) "
				"normal(%.1f,%.1f,%.1f) color(%.0f,%.0f,%.0f)\n", i,
				obj->shape.plane.point.x, obj->shape.plane.point.y,
				obj->shape.plane.point.z, obj->shape.plane.normal.x,
				obj->shape.plane.normal.y, obj->shape.plane.normal.z,
				obj->shape.plane.color.x, obj->shape.plane.color.y,
				obj->shape.plane.color.z);
		else if (obj->type == CYLINDER)
			printf("  [%d] CYLINDER: center(%.1f,%.1f,%.1f) "
				"d=%.1f h=%.1f color(%.0f,%.0f,%.0f)\n", i,
				obj->shape.cylinder.center.x,
				obj->shape.cylinder.center.y,
				obj->shape.cylinder.center.z,
				obj->shape.cylinder.diameter,
				obj->shape.cylinder.height,
				obj->shape.cylinder.color.x,
				obj->shape.cylinder.color.y,
				obj->shape.cylinder.color.z);
		obj = obj->next;
	}
}

static void	print_scene(t_scene *scene)
{
	t_light	*light;
	int		i;

	printf("=== SCENE PARSE TEST ===\n\n");
	printf("Ambient: intensity=%.1f color(%.0f,%.0f,%.0f)\n",
		scene->ambient.intensity,
		scene->ambient.color.x, scene->ambient.color.y,
		scene->ambient.color.z);
	printf("Camera: pos(%.1f,%.1f,%.1f) fov=%.1f\n",
		scene->camera.position.x, scene->camera.position.y,
		scene->camera.position.z, scene->camera.fov);
	light = scene->lights;
	i = 0;
	while (light)
	{
		printf("Light[%d]: pos(%.1f,%.1f,%.1f) brightness=%.1f "
			"color(%.0f,%.0f,%.0f)\n", ++i,
			light->position.x, light->position.y, light->position.z,
			light->brightness,
			light->color.x, light->color.y, light->color.z);
		light = light->next;
	}
	printf("Objects:\n");
	print_objects(scene->objects);
	printf("\n=== END ===\n");
}

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
	print_scene(&minirt.scene);
	return (0);
}
