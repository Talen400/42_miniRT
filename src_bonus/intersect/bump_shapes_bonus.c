/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_shapes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 00:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026/03/04 00:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt_bonus.h"
#include "intersect_bonus.h"

void	bump_sphere(t_vec3 point, t_object *obj, t_vec3 *normal)
{
	t_vec3			local;
	t_bump_frame	f;
	double			u;
	double			v;

	local = vec3_normalize(vec3_subtract(point, obj->shape.sphere.center));
	u = (atan2(local.z, local.x) / (2.0 * PI) + 0.5);
	v = acos(fmax(-1.0, fmin(1.0, local.y))) / PI;
	f.tangent = vec3_normalize(vec3_cross((t_vec3){0, 1, 0}, *normal));
	if (vec3_length(f.tangent) < EPSILON)
		f.tangent = (t_vec3){1, 0, 0};
	f.bitangent = vec3_cross(*normal, f.tangent);
	f.scale = obj->bump_scale;
	if (obj->bump_map)
		calc_grad_image(&f, obj->bump_map, u, v);
	else
		calc_grad_proc(&f, u, v);
	apply_bump(normal, &f);
}

void	bump_plane(t_vec3 point, t_object *obj, t_vec3 *normal)
{
	t_vec3			n;
	t_bump_frame	f;
	double			u;
	double			v;

	n = obj->shape.plane.normal;
	if (fabs(n.x) < 0.9)
		f.tangent = vec3_normalize(vec3_cross(n, (t_vec3){1, 0, 0}));
	else
		f.tangent = vec3_normalize(vec3_cross(n, (t_vec3){0, 1, 0}));
	f.bitangent = vec3_cross(n, f.tangent);
	u = vec3_dot(point, f.tangent) * obj->checker_scale;
	v = vec3_dot(point, f.bitangent) * obj->checker_scale;
	f.scale = obj->bump_scale;
	if (obj->bump_map)
		calc_grad_image(&f, obj->bump_map, fmod(fabs(u), 1.0),
			fmod(fabs(v), 1.0));
	else
		calc_grad_proc(&f, u, v);
	apply_bump(normal, &f);
}

void	bump_cylinder(t_vec3 point, t_object *obj, t_vec3 *normal)
{
	t_vec3			center_to_hit;
	t_vec3			radial;
	t_bump_frame	f;
	double			u;
	double			v;

	center_to_hit = vec3_subtract(point, obj->shape.cylinder.center);
	v = vec3_dot(center_to_hit, obj->shape.cylinder.axis);
	radial = vec3_normalize(vec3_subtract(center_to_hit,
				vec3_multiply(obj->shape.cylinder.axis, v)));
	f.tangent = vec3_cross(obj->shape.cylinder.axis, radial);
	f.bitangent = obj->shape.cylinder.axis;
	u = (atan2(radial.z, radial.x) / (2.0 * PI) + 0.5);
	v = v / obj->shape.cylinder.height;
	f.scale = obj->bump_scale;
	if (obj->bump_map)
		calc_grad_image(&f, obj->bump_map, u, fmod(fabs(v), 1.0));
	else
		calc_grad_proc(&f, u, v);
	apply_bump(normal, &f);
}

void	bump_cone(t_vec3 point, t_object *obj, t_vec3 *normal)
{
	t_vec3			apex_to_hit;
	t_vec3			radial;
	t_bump_frame	f;
	double			u;
	double			v;

	apex_to_hit = vec3_subtract(point, obj->shape.cone.apex);
	v = vec3_dot(apex_to_hit, obj->shape.cone.axis);
	radial = vec3_normalize(vec3_subtract(apex_to_hit,
				vec3_multiply(obj->shape.cone.axis, v)));
	f.tangent = vec3_cross(obj->shape.cone.axis, radial);
	f.bitangent = obj->shape.cone.axis;
	u = (atan2(radial.z, radial.x) / (2.0 * PI) + 0.5);
	v = v / obj->shape.cone.height;
	f.scale = obj->bump_scale;
	if (obj->bump_map)
		calc_grad_image(&f, obj->bump_map, u, fmod(fabs(v), 1.0));
	else
		calc_grad_proc(&f, u, v);
	apply_bump(normal, &f);
}
