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

bool	set_specular(t_object *obj, char **tokens, int base_count)
{
	double	ks;
	double	shininess;
 
    int size = ft_array_size((void **)tokens);
    if (size == base_count + 2)
    {
        ks = ft_atod(tokens[base_count]);
		if (ks < 0.0 || ks >= 1)
			return (false);
		obj->ks = ks;
        shininess = ft_atod(tokens[base_count + 1]);
		if (shininess < 1 || shininess > 500)
			return (false);
		obj->shininess = shininess;
    }
    else
    {
        obj->ks = KS_DEFAULT;
        obj->shininess = SHININESS_DEFAULT;
    }
	return (true);
}

t_object	*create_sphere_obj(t_sphere sphere, char **tokens)
{
	t_object	*obj;

	obj = (t_object *)malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->type = SPHERE;
	if(!set_specular(obj, tokens, SP_NARGS))
	{
		free(obj);
		return (NULL);
	}
	obj->shape.sphere = sphere;
	obj->next = NULL;
	
	return (obj);
}

t_object	*create_plane_obj(t_plane plane, char **tokens)
{
	t_object	*obj;

	obj = (t_object *)malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->type = PLANE;
	if (!set_specular(obj, tokens, PL_NARGS))
	{
		free(obj);
		return (NULL);
	}
	obj->shape.plane = plane;
	obj->next = NULL;
	return (obj);
}

t_object	*create_cylinder_obj(t_cylinder cylinder, char **tokens)
{
	t_object	*obj;

	obj = (t_object *)malloc(sizeof(t_object));
	if (!obj)
		return (NULL);
	obj->type = CYLINDER;
	if (!set_specular(obj, tokens, CY_NARGS))
	{
		free(obj);
		return (NULL);
	}
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
