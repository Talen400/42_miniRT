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

#include "miniRt_bonus.h"
#include "intersect_bonus.h"

void	apply_bump(t_vec3 *normal, t_bump_frame *f)
{
	t_vec3	perturbed;

	perturbed = vec3_add(*normal,
			vec3_add(vec3_multiply(f->tangent, f->du),
				vec3_multiply(f->bitangent, f->dv)));
	*normal = vec3_normalize(perturbed);
}

double	sample_map(t_bump_map *bm, double u, double v)
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

void	calc_grad_proc(t_bump_frame *f, double u, double v)
{
	f->du = cos(u * 2.0 * PI) * sin(v * 2.0 * PI) * f->scale;
	f->dv = sin(u * 2.0 * PI) * cos(v * 2.0 * PI) * f->scale;
}

void	calc_grad_image(t_bump_frame *f, t_bump_map *bm, double u, double v)
{
	double	eps;
	double	h0;

	eps = 1.0 / bm->width;
	h0 = sample_map(bm, u, v);
	f->du = (sample_map(bm, u + eps, v) - h0) * f->scale;
	f->dv = (sample_map(bm, u, v + eps) - h0) * f->scale;
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
