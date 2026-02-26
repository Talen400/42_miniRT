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

#include "miniRt.h"

t_scene	scene_init(void)
{
	t_scene	scene;

	scene.camera = (t_camera){};
	scene.ambient = (t_ambient){};
	scene.lights = NULL;
	scene.objects = NULL;
	scene.width = WIDTH;
	scene.height = HEIGHT;
	return (scene);
}

void	destroy_scene(t_scene *scene)
{
	t_light		*current_light;
	t_object	*current_obj;
	t_light		*next_light;
	t_object	*next_obj;

	if (!scene)
		return ;
	current_light = scene->lights;
	while (current_light)
	{
		next_light = current_light->next;
		free(current_light);
		current_light = next_light;
	}
	current_obj = scene->objects;
	while (current_obj)
	{
		next_obj = current_obj->next;
		if (current_obj->bump_map)
		{
			free(current_obj->bump_map->data);
			free(current_obj->bump_map);
		}
		free(current_obj);
		current_obj = next_obj;
	}
}

void	add_object_to_scene(t_scene *scene, t_object *obj)
{
	t_object	*current;

	if (!scene || !obj)
		return ;
	if (!scene->objects)
	{
		scene->objects = obj;
		return ;
	}
	current = scene->objects;
	while (current->next)
		current = current->next;
	current->next = obj;
}
