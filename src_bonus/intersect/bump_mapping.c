/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_mapping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/21 00:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "intersect.h"

typedef struct s_bump_frame
{
	t_vec3	tangent;
	t_vec3	bitangent;
	double	du;
	double	dv;
	double	scale;
}	t_bump_frame;

static void	apply_bump(t_vec3 *normal, t_bump_frame *f)
{
	t_vec3	perturbed;

	perturbed = vec3_add(*normal,
			vec3_add(vec3_multiply(f->tangent, f->du),
				vec3_multiply(f->bitangent, f->dv)));
	*normal = vec3_normalize(perturbed);
}

static double	sample_map(t_bump_map *bm, double u, double v)
{
	unsigned int	px;
	unsigned int	py;
	unsigned int	idx;
	unsigned char	*p;

	px = (unsigned int)(u * bm->width) % bm->width;
	py = (unsigned int)(v * bm->height) % bm->height;
	idx = (py * bm->width + px) * 4;
	p = bm->data + idx;
	return ((p[0] + p[1] + p[2]) / (3.0 * 255.0));
}

static void	calc_grad_proc(t_bump_frame *f, double u, double v)
{
	f->du = cos(u * 2.0 * PI) * sin(v * 2.0 * PI) * f->scale;
	f->dv = sin(u * 2.0 * PI) * cos(v * 2.0 * PI) * f->scale;
}

static void	calc_grad_image(t_bump_frame *f, t_bump_map *bm, double u,
	double v)
{
	double	eps;
	double	h0;

	eps = 1.0 / bm->width;
	h0 = sample_map(bm, u, v);
	f->du = (sample_map(bm, u + eps, v) - h0) * f->scale;
	f->dv = (sample_map(bm, u, v + eps) - h0) * f->scale;
}

static void	bump_sphere(t_vec3 point, t_object *obj, t_vec3 *normal)
{
	t_vec3		local;
	t_bump_frame	f;
	double		u;
	double		v;

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

static void	bump_plane(t_vec3 point, t_object *obj, t_vec3 *normal)
{
	t_vec3		n;
	t_bump_frame	f;
	double		u;
	double		v;

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

static void	bump_cylinder(t_vec3 point, t_object *obj, t_vec3 *normal)
{
	t_vec3		center_to_hit;
	t_vec3		radial;
	t_bump_frame	f;
	double		u;
	double		v;

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

static void	bump_cone(t_vec3 point, t_object *obj, t_vec3 *normal)
{
	t_vec3		apex_to_hit;
	t_vec3		radial;
	t_bump_frame	f;
	double		u;
	double		v;

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

void	perturb_normal(t_vec3 point, t_vec3 *normal, t_object *obj)
{
	if (obj->bump_scale <= 0.0)
		return ;
	if (obj->type == SPHERE)
		bump_sphere(point, obj, normal);
	else if (obj->type == PLANE)
		bump_plane(point, obj, normal);
	else if (obj->type == CYLINDER)
		bump_cylinder(point, obj, normal);
	else if (obj->type == CONE)
		bump_cone(point, obj, normal);
}