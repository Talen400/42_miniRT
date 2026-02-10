/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-09 00:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-09 00:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"

static t_color	color_mult(t_color a, t_color b)
{
	return (vec3_create(a.x * b.x, a.y * b.y, a.z * b.z));
}

static t_color	color_add(t_color a, t_color b)
{
	return (vec3_add(a, b));
}

static t_color	color_scale(t_color c, double s)
{
	return (vec3_multiply(c, s));
}

static bool	in_shadow(t_scene *scene, t_point3 point, t_vec3 light_dir,
		double light_dist)
{
	t_ray			shadow_ray;
	t_object		*obj;
	t_hit_record	tmp;

	shadow_ray.origin = vec3_add(point, vec3_multiply(light_dir, EPSILON));
	shadow_ray.direction = light_dir;
	obj = scene->objects;
	while (obj)
	{
		if (obj->type == SPHERE && hit_sphere(shadow_ray, obj->shape.sphere,
				T_MIN, light_dist, &tmp))
			return (true);
		else if (obj->type == PLANE && hit_plane(shadow_ray, obj->shape.plane,
				T_MIN, light_dist, &tmp))
			return (true);
		obj = obj->next;
	}
	return (false);
}

t_color	calculate_lighting(t_scene *scene, t_hit_record *rec)
{
	t_color		result;
	t_color		diffuse;
	t_light		*light;
	t_vec3		light_dir;
	double		diff;
	double		light_dist;

	result = color_scale(color_mult(scene->ambient.color, rec->color),
			scene->ambient.intensity);
	light = scene->lights;
	while (light)
	{
		light_dir = vec3_subtract(light->position, rec->point);
		light_dist = vec3_length(light_dir);
		light_dir = vec3_normalize(light_dir);
		if (!in_shadow(scene, rec->point, light_dir, light_dist))
		{
			diff = vec3_dot(rec->normal, light_dir);
			if (diff < 0)
				diff = 0;
			diffuse = color_scale(color_mult(light->color, rec->color),
					diff * light->brightness);
			result = color_add(result, diffuse);
		}
		light = light->next;
	}
	if (result.x > 1.0)
		result.x = 1.0;
	if (result.y > 1.0)
		result.y = 1.0;
	if (result.z > 1.0)
		result.z = 1.0;
	return (result);
}