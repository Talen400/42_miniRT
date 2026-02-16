/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-10 15:35:09 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-10 15:35:09 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "intersect.h"

static t_color	calculate_light_contribution(t_light *light,
				t_hit_record *rec, double diffuse)
{
	t_color	lc;

	lc.r = (uint8_t)fmin(255,
			rec->color.r * light->color.r / 255.0 * diffuse);
	lc.g = (uint8_t)fmin(255,
			rec->color.g * light->color.g / 255.0 * diffuse);
	lc.b = (uint8_t)fmin(255,
			rec->color.b * light->color.b / 255.0 * diffuse);
	lc.a = 255;
	return (lc);
}

static bool	in_shadow(t_scene *scene, t_vec3 to_light,
		t_hit_record *rec, t_vec3 light_dir)
{
	t_ray			shadow_ray;
	t_hit_record	shadow_rec;
	double			light_distance;

	light_distance = vec3_length(to_light);
	shadow_ray.origin = vec3_add(rec->point,
			vec3_multiply(rec->normal, EPSILON));
	shadow_ray.direction = light_dir;
	if (hit_scene(&shadow_ray, scene->objects, &shadow_rec, 0.1))
	{
		if (shadow_rec.t < light_distance)
			return (true);
	}
	return (false);
}

static t_color	calculate_specular(t_scene *scene, t_hit_record *rec,
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

static t_color	apply_light(t_scene *scene, t_hit_record *rec,
				t_light *light)
{
	t_vec3	to_light;
	t_vec3	light_dir;
	double	diffuse;
	t_color	result;

	to_light = vec3_subtract(light->position, rec->point);
	light_dir = vec3_normalize(to_light);
	if (in_shadow(scene, to_light, rec, light_dir))
		return ((t_color){0, 0, 0, 0});
	diffuse = fmax(0.0, vec3_dot(rec->normal, light_dir));
	diffuse *= light->brightness;
	result = calculate_light_contribution(light, rec, diffuse);
	result = color_add(result,
			calculate_specular(scene, rec, light_dir, light));
	return (result);
}

t_color	calculate_lighting(t_scene *scene, t_hit_record *rec)
{
	t_color	final_color;
	t_light	*light;

	final_color = color_scale(rec->color, scene->ambient.intensity);
	light = scene->lights;
	while (light)
	{
		final_color = color_add(final_color,
				apply_light(scene, rec, light));
		light = light->next;
	}
	return (color_clamp(final_color));
}
