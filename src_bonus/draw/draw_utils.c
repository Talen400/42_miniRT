/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 00:00:00 by tlavared          #+#    #+#             */
/*   Updated: 2026/02/26 00:00:00 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "mlx_rt.h"
#include <stdlib.h>

static double	random_double(void)
{
	return (rand() / (RAND_MAX + 1.0));
}

static t_ray	pixel_ray(t_camera *cam, double x, double y, t_scene *scene)
{
	double	u;
	double	v;
	t_vec3	pixel_center;
	t_vec3	dir;

	u = x / (scene->width - 1.0);
	v = y / (scene->height - 1.0);
	pixel_center = vec3_add(cam->lower_left_corner,
			vec3_add(vec3_multiply(cam->horizontal, u),
				vec3_multiply(cam->vertical, v)));
	dir = vec3_normalize(vec3_subtract(pixel_center, cam->position));
	return (ray_create(cam->position, dir));
}

t_color	pixel_color_aa(t_minirt *minirt, int x, int y)
{
	double	r_sum;
	double	g_sum;
	double	b_sum;
	t_ray	ray;
	t_color	sample_color;
	int		i;

	r_sum = 0.0;
	g_sum = 0.0;
	b_sum = 0.0;
	i = 0;
	while (i < SAMPLES_PER_PIXEL)
	{
		ray = pixel_ray(&minirt->scene.camera,
			x + random_double(),
			y + random_double(),
			&minirt->scene);
		sample_color = ray_color(&ray, &minirt->scene);
		r_sum += sample_color.r;
		g_sum += sample_color.g;
		b_sum += sample_color.b;
		i++;
	}
	sample_color.r = (uint8_t)(r_sum / SAMPLES_PER_PIXEL);
	sample_color.g = (uint8_t)(g_sum / SAMPLES_PER_PIXEL);
	sample_color.b = (uint8_t)(b_sum / SAMPLES_PER_PIXEL);
	sample_color.a = 255;
	return (sample_color);
}
