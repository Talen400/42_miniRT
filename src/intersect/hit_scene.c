/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 00:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/11 00:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "intersect.h"

static double	hit_object(t_ray *r, t_object *obj)
{
	if (obj->type == SPHERE)
		return (hit_sphere(r, obj));
	if (obj->type == PLANE)
		return (hit_plane(r, obj));
	if (obj->type == CYLINDER)
		return (hit_cylinder(r, obj));
	return (-1.0);
}

bool	hit_scene(t_ray *r, t_object *objects,
		t_hit_record *rec, double t_min)
{
	t_object	*obj;
	double		closest;
	double		t;
	bool		hit_any;

	closest = T_MAX;
	hit_any = false;
	obj = objects;
	while (obj)
	{
		t = hit_object(r, obj);
		if (t > t_min && t < closest)
		{
			closest = t;
			hit_any = true;
			fill_hit_record(rec, r, obj, t);
		}
		obj = obj->next;
	}
	return (hit_any);
}
