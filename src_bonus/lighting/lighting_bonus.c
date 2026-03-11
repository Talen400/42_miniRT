/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-10 15:35:09 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/21 20:26:04 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt_bonus.h"
#include "intersect_bonus.h"

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
			vec3_multiply(rec->normal, T_MIN));
	shadow_ray.direction = light_dir;
	if (hit_scene(&shadow_ray, scene->objects, &shadow_rec, T_MIN))
	{
		if (shadow_rec.t < light_distance)
			return (true);
	}
	return (false);
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
		return ((t_color){0, 0, 0, 255});
	diffuse = fmax(0.0, vec3_dot(rec->normal, light_dir));
	diffuse *= light->brightness * rec->object->kd;
	result = calculate_light_contribution(light, rec, diffuse);
	result = color_add(result,
			calculate_specular(scene, rec, light_dir, light));
	return (result);
}

static t_color	calculate_ambient(t_scene *scene, t_hit_record *rec)
{
	t_color	ambient;

	ambient.r = (uint8_t)fmin(255, rec->color.r
			* scene->ambient.color.r / 255.0
			* scene->ambient.intensity * rec->object->ka);
	ambient.g = (uint8_t)fmin(255, rec->color.g
			* scene->ambient.color.g / 255.0
			* scene->ambient.intensity * rec->object->ka);
	ambient.b = (uint8_t)fmin(255, rec->color.b
			* scene->ambient.color.b / 255.0
			* scene->ambient.intensity * rec->object->ka);
	ambient.a = 255;
	return (ambient);
}

t_color	calculate_lighting(t_scene *scene, t_hit_record *rec, t_ray *ray,
		int depth)
{
	t_color	direct;
	t_color	reflected;
	t_light	*light;

	direct = (t_color){0, 0, 0, 255};
	light = scene->lights;
	while (light)
	{
		direct = color_add(direct, apply_light(scene, rec, light));
		light = light->next;
	}
	reflected = calculate_reflection(scene, rec, ray, depth);
	direct = color_add(
			color_scale(direct, 1.0 - rec->object->reflectivity),
			color_scale(reflected, rec->object->reflectivity));
	return (color_clamp(color_add(direct, calculate_ambient(scene, rec))));
}
