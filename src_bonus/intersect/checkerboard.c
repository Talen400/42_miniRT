/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/21 00:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "intersect.h"

static t_color	pick_checker(double u, double v, t_object *obj)
{
	int	check;

	check = ((int)floor(u) + (int)floor(v)) % 2;
	if (check < 0)
		check += 2;
	if (check == 0)
		return (get_object_color(obj));
	return (obj->color2);
}

static t_color	checker_sphere(t_vec3 point, t_object *obj)
{
	t_vec3	local;
	double	u;
	double	v;
	double	s;

	local = vec3_normalize(vec3_subtract(point, obj->shape.sphere.center));
	s = obj->checker_scale;
	u = (atan2(local.z, local.x) / (2.0 * PI) + 0.5) * s;
	v = acos(fmax(-1.0, fmin(1.0, local.y))) / PI * s;
	return (pick_checker(u, v, obj));
}

static t_color	checker_plane(t_vec3 point, t_object *obj)
{
	t_vec3	n;
	t_vec3	tangent;
	t_vec3	bitangent;
	double	u;
	double	v;
	double	s;

	n = obj->shape.plane.normal;
	if (fabs(n.x) < 0.9)
		tangent = vec3_normalize(vec3_cross(n, (t_vec3){1, 0, 0}));
	else
		tangent = vec3_normalize(vec3_cross(n, (t_vec3){0, 1, 0}));
	bitangent = vec3_cross(n, tangent);
	s = obj->checker_scale;
	u = vec3_dot(point, tangent) * s;
	v = vec3_dot(point, bitangent) * s;
	return (pick_checker(u, v, obj));
}

static t_color	checker_cylinder(t_vec3 point, t_object *obj)
{
	t_vec3	center_to_hit;
	t_vec3	radial;
	double	u;
	double	v;
	double	s;

	center_to_hit = vec3_subtract(point, obj->shape.cylinder.center);
	v = vec3_dot(center_to_hit, obj->shape.cylinder.axis);
	radial = vec3_normalize(vec3_subtract(center_to_hit,
				vec3_multiply(obj->shape.cylinder.axis, v)));
	s = obj->checker_scale;
	u = (atan2(radial.z, radial.x) / (2.0 * PI) + 0.5) * s;
	v = v / obj->shape.cylinder.height * s;
	return (pick_checker(u, v, obj));
}

static t_color	checker_cone(t_vec3 point, t_object *obj)
{
	t_vec3	apex_to_hit;
	t_vec3	radial;
	double	h;
	double	u;
	double	v;
	double	s;

	apex_to_hit = vec3_subtract(point, obj->shape.cone.apex);
	h = vec3_dot(apex_to_hit, obj->shape.cone.axis);
	radial = vec3_normalize(vec3_subtract(apex_to_hit,
				vec3_multiply(obj->shape.cone.axis, h)));
	s = obj->checker_scale;
	u = (atan2(radial.z, radial.x) / (2.0 * PI) + 0.5) * s;
	v = h / obj->shape.cone.height * s;
	return (pick_checker(u, v, obj));
}

t_color	get_surface_color(t_vec3 point, t_object *obj)
{
	if (!obj->has_checker)
		return (get_object_color(obj));
	if (obj->type == SPHERE)
		return (checker_sphere(point, obj));
	if (obj->type == PLANE)
		return (checker_plane(point, obj));
	if (obj->type == CYLINDER)
		return (checker_cylinder(point, obj));
	if (obj->type == CONE)
		return (checker_cone(point, obj));
	return (get_object_color(obj));
}