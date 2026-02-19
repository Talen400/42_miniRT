/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-19 18:34:48 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-19 18:34:48 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "intersect.h"

static int	check_cone_height(t_ray *r, t_object *obj,
	t_quadratic_equation values)
{
	t_vec3	apex_to_hit;
	double	projection;

	apex_to_hit = vec3_subtract(
			vec3_add(r->origin, vec3_multiply(r->direction, values.t)),
			obj->shape.cone.apex);
	projection = vec3_dot(apex_to_hit, obj->shape.cone.axis);
	if (projection < 0 || projection > obj->shape.cone.height)
		return (-1);
	return (1);
}

static double	calc_quadratic_cone(t_cone_calc cc, t_ray *r, t_object *obj)
{
	t_quadratic_equation	values;
	double					k;

	k = obj->shape.cone.half_angle;
	values.a = vec3_dot(cc.w, cc.w) - k * k * cc.d_dot_a * cc.d_dot_a;
	if (fabs(values.a) < EPSILON)
		return (-1);
	values.b = 2 * (vec3_dot(cc.v, cc.w) - k * k * cc.x_dot_a * cc.d_dot_a);
	values.c = vec3_dot(cc.v, cc.v) - k * k * cc.x_dot_a * cc.x_dot_a;
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
	if (check_cone_height(r, obj, values) == -1)
		return (-1);
	return (values.t);
}

static double	calc_cone(t_ray *r, t_object *obj)
{
	t_cone_calc	cc;
	t_vec3		x;

	x = vec3_subtract(r->origin, obj->shape.cone.apex);
	cc.x_dot_a = vec3_dot(x, obj->shape.cone.axis);
	cc.d_dot_a = vec3_dot(r->direction, obj->shape.cone.axis);
	cc.v = vec3_subtract(x, vec3_multiply(obj->shape.cone.axis, cc.x_dot_a));
	cc.w = vec3_subtract(r->direction,
			vec3_multiply(obj->shape.cone.axis, cc.d_dot_a));
	return (calc_quadratic_cone(cc, r, obj));
}

double	hit_cone(t_ray *r, t_object *obj)
{
	double	t_body;
	double	t_base;
	double	closest;
	t_vec3	base_center;

	t_body = calc_cone(r, obj);
	base_center = vec3_add(obj->shape.cone.apex,
			vec3_multiply(obj->shape.cone.axis, obj->shape.cone.height));
	t_base = hit_cap(r, base_center,
			obj->shape.cone.axis, obj->shape.cone.radius);
	closest = -1;
	if (t_body > 0 && (closest < 0 || t_body < closest))
		closest = t_body;
	if (t_base > 0 && (closest < 0 || t_base < closest))
		closest = t_base;
	return (closest);
}
