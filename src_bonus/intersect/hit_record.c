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

static bool	is_cone_base(t_vec3 hit_point, t_object *obj)
{
	t_vec3	apex_to_hit;
	double	projection;

	apex_to_hit = vec3_subtract(hit_point, obj->shape.cone.apex);
	projection = vec3_dot(apex_to_hit, obj->shape.cone.axis);
	return (fabs(projection - obj->shape.cone.height) < CAP_EPSILON);
}

static t_vec3	cone_normal(t_vec3 hit_point, t_object *obj)
{
	t_vec3	apex_to_hit;
	double	h;
	t_vec3	point_on_axis;
	t_vec3	radial_unit;

	if (is_cone_base(hit_point, obj))
		return (obj->shape.cone.axis);
	apex_to_hit = vec3_subtract(hit_point, obj->shape.cone.apex);
	h = vec3_dot(apex_to_hit, obj->shape.cone.axis);
	point_on_axis = vec3_add(obj->shape.cone.apex,
			vec3_multiply(obj->shape.cone.axis, h));
	radial_unit = vec3_normalize(vec3_subtract(hit_point, point_on_axis));
	return (vec3_normalize(vec3_subtract(radial_unit,
				vec3_multiply(obj->shape.cone.axis,
					obj->shape.cone.half_angle))));
}

static t_cylinder_surface	identify_cylinder_surface(t_vec3 hit_point,
		t_object *obj)
{
	t_vec3	center_to_hit;
	double	projection;

	center_to_hit = vec3_subtract(hit_point, obj->shape.cylinder.center);
	projection = vec3_dot(center_to_hit, obj->shape.cylinder.axis);
	if (fabs(projection) < CAP_EPSILON)
		return (CYL_BOTTOM_CAP);
	if (fabs(projection - obj->shape.cylinder.height) < CAP_EPSILON)
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

t_color	get_object_color(t_object *obj)
{
	if (obj->type == SPHERE)
		return (obj->shape.sphere.color);
	if (obj->type == PLANE)
		return (obj->shape.plane.color);
	if (obj->type == CYLINDER)
		return (obj->shape.cylinder.color);
	if (obj->type == CONE)
		return (obj->shape.cone.color);
	error_exit("get_object_color: unknown object type");
	return ((t_color){0, 0, 0, 255});
}

void	fill_hit_record(t_hit_record *rec, t_ray *r, t_object *obj, double t)
{
	rec->object = obj;
	rec->t = t;
	rec->point = vec3_add(r->origin,
			vec3_multiply(r->direction, t));
	if (obj->type == SPHERE)
	{
		rec->normal = vec3_normalize(vec3_subtract(rec->point,
					obj->shape.sphere.center));
	}
	else if (obj->type == PLANE)
	{
		if (vec3_dot(r->direction, obj->shape.plane.normal) > 0)
			rec->normal = vec3_multiply(obj->shape.plane.normal, -1.0);
		else
			rec->normal = obj->shape.plane.normal;
	}
	else if (obj->type == CYLINDER)
	{
		rec->cylinder_surface = identify_cylinder_surface(rec->point, obj);
		rec->normal = cylinder_normal(rec->point, obj, rec->cylinder_surface);
	}
	else if (obj->type == CONE)
		rec->normal = cone_normal(rec->point, obj);
	perturb_normal(rec->point, &rec->normal, obj);
	rec->color = get_surface_color(rec->point, obj);
}
