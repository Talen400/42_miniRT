/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting_reflect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-04 00:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026-03-04 00:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt_bonus.h"
#include "intersect_bonus.h"

t_color	calculate_specular(t_scene *scene, t_hit_record *rec,
			t_vec3 light_dir, t_light *light)
{
	t_vec3	view_dir;
	t_vec3	reflect_dir;
	double	spec;
	t_color	spec_color;

	view_dir = vec3_normalize(
			vec3_subtract(scene->camera.position, rec->point));
	reflect_dir = vec3_subtract(
			vec3_multiply(rec->normal,
				2.0 * vec3_dot(rec->normal, light_dir)), light_dir);
	spec = pow(fmax(0.0, vec3_dot(view_dir, reflect_dir)),
			rec->object->shininess);
	spec *= rec->object->ks * light->brightness;
	spec_color.r = (uint8_t)fmin(255, light->color.r * spec);
	spec_color.g = (uint8_t)fmin(255, light->color.g * spec);
	spec_color.b = (uint8_t)fmin(255, light->color.b * spec);
	spec_color.a = 255;
	return (spec_color);
}

t_color	calculate_reflection(t_scene *scene, t_hit_record *rec,
		t_ray *ray, int depth)
{
	t_ray			reflect_ray;
	t_hit_record	reflect_rec;
	t_vec3			reflect_dir;

	if (rec->object->reflectivity <= 0.0 || depth <= 0)
		return ((t_color){0, 0, 0, 255});
	reflect_dir = vec3_subtract(
			vec3_multiply(rec->normal,
				2.0 * vec3_dot(rec->normal,
					vec3_negate(ray->direction))),
			vec3_negate(ray->direction));
	reflect_ray.origin = vec3_add(rec->point,
			vec3_multiply(rec->normal, T_MIN));
	reflect_ray.direction = vec3_normalize(reflect_dir);
	if (!hit_scene(&reflect_ray, scene->objects, &reflect_rec, T_MIN))
		return (sky_color(&reflect_ray));
	return (calculate_lighting(scene, &reflect_rec, &reflect_ray, depth - 1));
}
