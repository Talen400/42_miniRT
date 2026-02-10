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
	t_color	final_color;
	t_color	light_contribution;
	t_light	*current_light;
	t_vec3	light_dir;
	double	diffuse;

	final_color = color_scale(rec->color, scene->ambient.intensity);
	current_light = scene->lights;
	while (current_light)
	{
		light_dir = vec3_normalize(
			vec3_subtract(current_light->position, rec->point));
		diffuse = fmax(0.0, vec3_dot(rec->normal, light_dir));
		diffuse *= current_light->brightness;
		light_contribution = color_scale(rec->color, diffuse);
		final_color = color_add(final_color, light_contribution);
		current_light = current_light->next;
	}
	final_color.r = fmin(255, final_color.r);
	final_color.g = fmin(255, final_color.g);
	final_color.b = fmin(255, final_color.b);
	final_color.a = 255;
    return (final_color);
}
