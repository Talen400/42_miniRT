/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_record.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 00:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/11 00:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "intersect.h"

static t_vec3	cylinder_normal(void)
{
	return (vec3_create(0, 0, 0));
}

void	fill_hit_record(t_hit_record *rec, t_ray *r, t_object *obj, double t)
{
	rec->t = t;
	rec->point = vec3_add(r->origin,
			vec3_multiply(r->direction, t));
	if (obj->type == SPHERE)
	{
		rec->normal = vec3_normalize(vec3_subtract(rec->point,
					obj->shape.sphere.center));
		rec->color = obj->shape.sphere.color;
	}
	else if (obj->type == PLANE)
	{
		rec->normal = obj->shape.plane.normal;
		rec->color = obj->shape.plane.color;
	}
	else if (obj->type == CYLINDER)
	{
		rec->normal = cylinder_normal();
		rec->color = obj->shape.cylinder.color;
	}
}
