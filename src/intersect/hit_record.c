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

static t_cylinder_surface	identify_cylinder_surface(t_vec3 hit_point,
		t_object *obj)
{
	t_vec3	center_to_hit;
	double	projection;

	center_to_hit = vec3_subtract(hit_point, obj->shape.cylinder.center);
	projection = vec3_dot(center_to_hit, obj->shape.cylinder.axis);
	if (fabs(projection) < EPSILON)
		return (CYL_BOTTOM_CAP);
	if (fabs(projection - obj->shape.cylinder.height) < EPSILON)
		return (CYL_TOP_CAP);
	return (CYL_BODY);
}

static t_vec3	cylinder_normal(t_vec3 hit_point, t_object *obj,
	t_cylinder_surface surface)
{
	t_vec3	center_to_hit;
	t_vec3	point_on_axis;
	double	projection;

	if (surface == CYL_BOTTOM_CAP)
		return (vec3_multiply(obj->shape.cylinder.axis, -1.0));
	else if (surface == CYL_TOP_CAP)
		return (obj->shape.cylinder.axis);
	else
	{
		center_to_hit = vec3_subtract(hit_point, obj->shape.cylinder.center);
		projection = vec3_dot(center_to_hit, obj->shape.cylinder.axis);
		point_on_axis = vec3_add(obj->shape.cylinder.center,
				vec3_multiply(obj->shape.cylinder.axis, projection));
		return (vec3_normalize(vec3_subtract(hit_point, point_on_axis)));
	}
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
		rec->cylinder_surface = identify_cylinder_surface(rec->point, obj);
		rec->normal = cylinder_normal(rec->point, obj, rec->cylinder_surface);
		rec->color = obj->shape.cylinder.color;
	}
}
