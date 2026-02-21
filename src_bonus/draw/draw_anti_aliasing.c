/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_anti_aliasing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 01:44:49 by tlavared          #+#    #+#             */
/*   Updated: 2026/02/21 20:17:38 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/miniRt.h"
#include "../include_bonus/mlx_rt.h"
#include "../include_bonus/color.h"
#include "../include_bonus/intersect.h"
#include <stdlib.h>

#define SAMPLES_PER_PIXEL 64

/*
 *
 * Uma alternativa para o rand().
 *
 * Static e inline para o compilador otimizar a operação
 *
 * É static por que o valor precisa mudar a cada chamada
*/
static inline	int fast_rand(void)
{
	static unsigned int seed = 12345;

	seed = 214013 * seed + 2531011;
	return ((seed >> 16) &  0x7FFF);
}

static double	random_double(void)
{
	return (fast_rand() / (double ) 0x7FFF);
}

t_color	sky_color(t_ray *r)
{
	t_vec3	unit_dir;
	double	t;
	t_color	white;
	t_color	blue;

	unit_dir = r->direction;
	white = init_color(0xFFFFFFFF);
	blue = init_color(0x0066FFFF);
	t = 0.5 * (unit_dir.y + 1.0);
	return (color_add(
			color_scale(white, 1.0 - t),
			color_scale(blue, t)));
}

static t_color	ray_color(t_ray *r, t_scene *scene)
{
	t_hit_record	rec;
	t_color			norm_color;

	if (hit_scene(r, scene->objects, &rec, T_MIN))
	{
		norm_color = calculate_lighting(scene, &rec, r,
			REFLECTION_DEPTH);
		return (norm_color);
	}
	return (sky_color(r));
}

// MUDANÇA: agora aceita double em vez de int
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

// NOVA FUNÇÃO: anti-aliasing
static t_color	pixel_color_aa(t_minirt *minirt, int x, int y)
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
	sample_color.r = r_sum / SAMPLES_PER_PIXEL;
	sample_color.g = g_sum / SAMPLES_PER_PIXEL;
	sample_color.b = b_sum / SAMPLES_PER_PIXEL;
	sample_color.a = 255;
	return (sample_color);
}


static void	drawing(t_minirt *minirt)
{
	int		x;
	int		y;
	t_color	color;

	y = 0;
	while (y < minirt->scene.height)
	{
		x = 0;
		while (x < minirt->scene.width)
		{
			color = pixel_color_aa(minirt, x, y);
			put(minirt, x, y, color_to_int32(color));
			x++;
		}
		y++;
	}
}

int	draw(t_minirt *minirt)
{
	ft_clearimg(minirt);
	drawing(minirt);
	return (0);
}
