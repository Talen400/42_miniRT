/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 00:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/11 00:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "intersect.h"

static double	calc_plane(t_vec3 point, t_vec3 normal, t_ray *r)
{
	double	denom;
	double	t;

	denom = vec3_dot(r->direction, normal);
	if (fabs(denom) < EPSILON)
		return (-1);
	t = vec3_dot(vec3_subtract(point, r->origin), normal) / denom;
	if (t < 0)
		return (-1);
	return (t);
}

double	hit_plane(t_ray *r, t_object *obj)
{
	return (calc_plane(obj->shape.plane.point, obj->shape.plane.normal, r));
}
