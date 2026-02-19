/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-19 18:43:17 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-19 18:43:17 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "intersect.h"

double	hit_cap(t_ray *r, t_vec3 cap_center, t_vec3 axis, double radius)
{
	double	denom;
	double	t;
	t_vec3	hit_point;
	t_vec3	center_to_hit;
	double	distance_squared;

	denom = vec3_dot(r->direction, axis);
	if (fabs(denom) < EPSILON)
		return (-1);
	t = vec3_dot(vec3_subtract(cap_center, r->origin), axis) / denom;
	if (t < 0)
		return (-1);
	hit_point = vec3_add(r->origin, vec3_multiply(r->direction, t));
	center_to_hit = vec3_subtract(hit_point, cap_center);
	distance_squared = vec3_dot(center_to_hit, center_to_hit);
	if (distance_squared > radius * radius)
		return (-1);
	return (t);
}
