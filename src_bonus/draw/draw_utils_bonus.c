/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 00:00:00 by tlavared          #+#    #+#             */
/*   Updated: 2026/03/04 18:29:24 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt_bonus.h"
#include "mlx_rt_bonus.h"
#include <stdlib.h>

static double	rand_pixel(unsigned int *seed)
{
	*seed = 214013 * (*seed) + 2531011;
	return ((*seed >> 16 & 0x7FFF) / (double)0x7FFF);
}

static t_ray	pixel_ray(t_camera *cam, double x, double y, t_scene *scene)
{
	double	u;
	double	v;
	t_vec3	pixel_center;
	t_vec3	dir;

	u = (0.5 + x) / (scene->width - 1.0);
	v = (0.5 + y) / (scene->height - 1.0);
	pixel_center = vec3_add(cam->lower_left_corner,
			vec3_add(vec3_multiply(cam->horizontal, u),
				vec3_multiply(cam->vertical, v)));
	dir = vec3_normalize(vec3_subtract(pixel_center, cam->position));
	return (ray_create(cam->position, dir));
}

static t_vec3	calc_aa_sample(t_minirt *minirt, int x, int y)
{
	t_ray			ray;
	t_color			sample_color;
	t_vec3			sum;
	int				i;
	unsigned int	seed;

	seed = (unsigned int)(y * minirt->scene.width + x) * 2654435761u;
	sum = (t_vec3){0, 0, 0};
	i = 0;
	while (i < SAMPLES_PER_PIXEL)
	{
		ray = pixel_ray(&minirt->scene.camera,
				x + rand_pixel(&seed) - 0.5,
				y + rand_pixel(&seed) - 0.5,
				&minirt->scene);
		sample_color = ray_color(&ray, &minirt->scene);
		sum.x += sample_color.r;
		sum.y += sample_color.g;
		sum.z += sample_color.b;
		i++;
	}
	return (sum);
}

t_color	pixel_color_aa(t_minirt *minirt, int x, int y)
{
	t_vec3	sum;
	t_color	result;

	sum = calc_aa_sample(minirt, x, y);
	result.r = (uint8_t)(sum.x / SAMPLES_PER_PIXEL);
	result.g = (uint8_t)(sum.y / SAMPLES_PER_PIXEL);
	result.b = (uint8_t)(sum.z / SAMPLES_PER_PIXEL);
	result.a = 255;
	return (result);
}
