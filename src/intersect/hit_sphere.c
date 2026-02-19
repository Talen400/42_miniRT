/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 00:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/11 00:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "intersect.h"

static double	calc_sphere(t_vec3 *center, double radius, t_ray *r)
{
	t_vec3	oc;
	double	a;
	double	h;
	double	c;
	double	discriminant;

	oc = vec3_subtract(*center, r->origin);
	a = vec3_dot(r->direction, r->direction);
	h = vec3_dot(r->direction, oc);
	c = vec3_dot(oc, oc) - radius * radius;
	discriminant = h * h - a * c;
	if (discriminant < 0)
		return (-1.0);
	else
		return ((h - sqrt(discriminant)) / a);
}

double	hit_sphere(t_ray *r, t_object *obj)
{
	return (calc_sphere(&obj->shape.sphere.center,
			obj->shape.sphere.radius, r));
}
