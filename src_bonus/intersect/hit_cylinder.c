/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 00:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/11 00:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "intersect.h"

static int	is_valid_cylinder_hit(t_ray *r, t_object *obj, t_quadratic_equation values)
{
	t_vec3	hit_point;
	double	projection;
	t_vec3	center_to_hit;

	hit_point = vec3_add(r->origin,
			vec3_multiply(r->direction, values.t));
	center_to_hit = vec3_subtract(hit_point, obj->shape.cylinder.center);
	projection = vec3_dot(center_to_hit, obj->shape.cylinder.axis);
	if (projection < 0 || projection > obj->shape.cylinder.height)
		return (-1);
	return (1);
}

static double	calc_quadratic_equation(t_vec3 w, t_vec3 v,
	t_ray *r, t_object *obj)
{
	t_quadratic_equation	values;

	values.a = vec3_dot(w, w);
	if (values.a < EPSILON)
		return (-1);
	values.b = 2 * vec3_dot(v, w);
	values.c = vec3_dot(v, v) - obj->shape.cylinder.radius
		* obj->shape.cylinder.radius;
	values.discriminant = values.b * values.b - 4 * values.a * values.c;
	if (values.discriminant < 0)
		return (-1);
	values.t = (-values.b - sqrt(values.discriminant)) / (2 * values.a);
	if (values.t < 0)
	{
		values.t = (-values.b + sqrt(values.discriminant)) / (2 * values.a);
		if (values.t < 0)
			return (-1);
	}
	if (is_valid_cylinder_hit(r, obj, values) == -1)
		return (-1);
	return (values.t);
}

static double	calc_cylinder(t_ray *r, t_object *obj)
{
	t_vec3	x;
	double	x_dot_a;
	double	d_dot_a;
	t_vec3	v;
	t_vec3	w;

	x = vec3_subtract(r->origin, obj->shape.cylinder.center);
	x_dot_a = vec3_dot(x, obj->shape.cylinder.axis);
	v = vec3_subtract(x,
			vec3_multiply(obj->shape.cylinder.axis, x_dot_a));
	d_dot_a = vec3_dot(r->direction, obj->shape.cylinder.axis);
	w = vec3_subtract(r->direction,
			vec3_multiply(obj->shape.cylinder.axis, d_dot_a));
	return (calc_quadratic_equation(w, v, r, obj));
}

double	hit_cylinder(t_ray *r, t_object *obj)
{
	double	t_body;
	double	t_bottom;
	double	t_top;
	double	closest;
	t_vec3	top_center;

	t_body = calc_cylinder(r, obj);
	top_center = vec3_add(obj->shape.cylinder.center,
			vec3_multiply(obj->shape.cylinder.axis,
				obj->shape.cylinder.height));
	t_bottom = hit_cap(r, obj->shape.cylinder.center,
			obj->shape.cylinder.axis, obj->shape.cylinder.radius);
	t_top = hit_cap(r, top_center,
			obj->shape.cylinder.axis, obj->shape.cylinder.radius);
	closest = -1;
	if (t_body > 0 && (closest < 0 || t_body < closest))
		closest = t_body;
	if (t_bottom > 0 && (closest < 0 || t_bottom < closest))
		closest = t_bottom;
	if (t_top > 0 && (closest < 0 || t_top < closest))
		closest = t_top;
	return (closest);
}
