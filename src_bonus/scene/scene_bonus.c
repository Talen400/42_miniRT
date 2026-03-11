/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-05 17:08:32 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-05 17:08:32 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt_bonus.h"

t_scene	scene_init(void)
{
	t_scene	scene;

	scene.camera = (t_camera){};
	scene.ambient = (t_ambient){};
	scene.lights = NULL;
	scene.objects = NULL;
	scene.obj_tail = NULL;
	scene.width = WIDTH;
	scene.height = HEIGHT;
	return (scene);
}

static void	destroy_objects(t_object *obj)
{
	t_object	*next;

	while (obj)
	{
		next = obj->next;
		if (obj->bump_map)
		{
			free(obj->bump_map->data);
			free(obj->bump_map);
		}
		free(obj);
		obj = next;
	}
}

static void	destroy_lights(t_light *light)
{
	t_light	*next;

	while (light)
	{
		next = light->next;
		free(light);
		light = next;
	}
}

void	destroy_scene(t_scene *scene)
{
	if (!scene)
		return ;
	destroy_lights(scene->lights);
	destroy_objects(scene->objects);
}

void	add_object_to_scene(t_scene *scene, t_object *obj)
{
	if (!scene || !obj)
		return ;
	if (!scene->objects)
		scene->objects = obj;
	else
		scene->obj_tail->next = obj;
	scene->obj_tail = obj;
}
