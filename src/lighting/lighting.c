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

t_color calculate_lighting(t_scene *scene, t_hit_record *rec)
{
    t_color         final_color;
    t_color         light_contribution;
    t_light         *current_light;
    t_vec3          light_dir;
    t_vec3          to_light;
    t_ray           shadow_ray;
    t_hit_record    shadow_rec;
    double          light_distance;
    double          diffuse;

	final_color = color_scale(rec->color, scene->ambient.intensity);
	current_light = scene->lights;
	while (current_light)
	{
		to_light = vec3_subtract(current_light->position, rec->point);
		light_distance = vec3_length(to_light);
		light_dir = vec3_normalize(to_light);
		// Shadow ray: origem levemente acima da superfície
		shadow_ray.origin = vec3_add(rec->point,
				vec3_multiply(rec->normal, EPSILON));
		shadow_ray.direction = light_dir;
		if (hit_scene_shadow(&shadow_ray, scene->objects, &shadow_rec))
		{
			if (shadow_rec.t < light_distance)
			{
				current_light = current_light->next;
				continue;
			}
		}
		diffuse = fmax(0.0, vec3_dot(rec->normal, light_dir));
		diffuse *= current_light->brightness;
		light_contribution.r = (uint8_t)fmin(255,
				rec->color.r * current_light->color.r / 255.0 * diffuse);
		light_contribution.g = (uint8_t)fmin(255,
				rec->color.g * current_light->color.g / 255.0 * diffuse);
		light_contribution.b = (uint8_t)fmin(255,
				rec->color.b * current_light->color.b / 255.0 * diffuse);
		light_contribution.a = 255;
		final_color = color_add(final_color, light_contribution);
		current_light = current_light->next;
	}
	final_color = color_clamp(final_color);
	return (final_color);
}
