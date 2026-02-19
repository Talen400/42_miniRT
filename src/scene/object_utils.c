/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 15:07:34 by rgregori          #+#    #+#             */
/*   Updated: 2026-01-29 15:07:34 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"

t_object	*create_sphere_obj(t_sphere sphere)
{
	t_object	*obj;

	obj = (t_object *)malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->type = SPHERE;
	obj->shape.sphere = sphere;
	obj->next = NULL;
	return (obj);
}

t_object	*create_plane_obj(t_plane plane)
{
	t_object	*obj;

	obj = (t_object *)malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->type = PLANE;
	obj->shape.plane = plane;
	obj->next = NULL;
	return (obj);
}

t_object	*create_cylinder_obj(t_cylinder cylinder)
{
	t_object	*obj;

	obj = (t_object *)malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->type = CYLINDER;
	obj->shape.cylinder = cylinder;
	obj->next = NULL;
	return (obj);
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
