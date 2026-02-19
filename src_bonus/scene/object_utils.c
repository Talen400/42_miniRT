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

static t_object	*create_object(t_object_type type, char **tokens, int nargs)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->type = type;
	obj->next = NULL;
	if (!set_extra_args(obj, tokens, nargs))
	{
		free(obj);
		return (NULL);
	}
	return (obj);
}

t_object	*create_sphere_obj(t_sphere sphere, char **tokens)
{
	t_object	*obj;

	obj = create_object(SPHERE, tokens, SP_NARGS);
	if (obj)
		obj->shape.sphere = sphere;
	return (obj);
}

t_object	*create_plane_obj(t_plane plane, char **tokens)
{
	t_object	*obj;

	obj = create_object(PLANE, tokens, PL_NARGS);
	if (obj)
		obj->shape.plane = plane;
	return (obj);
}

t_object	*create_cylinder_obj(t_cylinder cylinder, char **tokens)
{
	t_object	*obj;

	obj = create_object(CYLINDER, tokens, CY_NARGS);
	if (obj)
		obj->shape.cylinder = cylinder;
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
