/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 01:44:49 by tlavared          #+#    #+#             */
/*   Updated: 2026/02/02 19:34:25 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRt.h"
#include "../include/mlx_rt.h"
#include "../include/color.h"

static inline void	put(uint8_t *pixels, int x, int y,
		uint32_t color)
{
	int	index;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		index = (y * WIDTH + x) * 4;
		pixels[index + 0] = (color >> 24) & 0xFF;
		pixels[index + 1] = (color >> 16) & 0xFF;
		pixels[index + 2] = (color >> 8) & 0xFF;
		pixels[index + 3] = color & 0xFF;
	}
}

double	is_hit_sphere(t_vec3 *center, double radius, t_ray *r)
{
	t_vec3	oc;
	double	a;
	double	h;
	double	c;
	double	discriminant;

	oc = vec3_subtract(*center, r->origin);
	a = vec3_length(r->direction);
	h = vec3_dot(r->direction, oc);
	c = vec3_length(oc) - radius * radius;
	discriminant = h * h - a * c;
	if (discriminant < 0)
		return -1.0;
	else
		return ((h - sqrt(discriminant)) / a);
}



static t_color	ray_color(t_ray *r)
{
	t_vec3	unit_dir;
	double	t;
	t_color	white;
	t_color	blue;
	t_vec3	center;
	t_vec3	norm_surface;
	t_vec3	hit_intersec;
	t_color	norm_color;

	center = vec3_create(0, 0, -1);
	t = is_hit_sphere(&center, 0.5, r);
	if (t > 0.0)
	{
		hit_intersec = vec3_add(r->origin, vec3_multiply(r->direction, t));
		norm_surface = vec3_normalize(vec3_subtract(hit_intersec, center));
		norm_color.r = (norm_surface.x + 1.0) * 0.5 * 255;
		norm_color.g = (norm_surface.y + 1.0) * 0.5 * 255;
		norm_color.b = (norm_surface.z + 1.0) * 0.5 * 255;
		norm_color.a = 255.0;
		return (norm_color);
	}
	// Shy
	unit_dir = r->direction;
	white = init_color(0xFFFFFFFF);
	blue = init_color(0x0066FFFF);
	t = 0.5 * (unit_dir.y + 1.0);
	return (color_add(
				color_scale(white, 1.0 - t),
				color_scale(blue, t))
		);
}

static void	drawing(t_minirt *minirt)
{
	t_camera	camera;
	int			x;
	int			y;
	double		u;
	double		v;
	t_vec3		pixel_center;
	t_vec3		dir;
	t_ray		r;
	t_color		color;

	camera = minirt->scene.camera;
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			u = (0.5 + x) / (WIDTH - 1.0);
			v = (0.5 + y) / (HEIGHT - 1.0);
			pixel_center = vec3_add(camera.lower_left_corner,
					vec3_add(vec3_multiply(camera.horizontal, u), 
						vec3_multiply(camera.vertical, v)));
			dir = vec3_subtract(pixel_center, camera.position);
			dir = vec3_normalize(dir);
			r = ray_create(camera.position, dir);
			color = ray_color(&r);
			put(minirt->mlx.pixels, x, y, color_to_int32(color));
			x++;
		}
		y++;
	}
}

int	draw(t_minirt *minirt)
{
	ft_clearimg(minirt->mlx.pixels);
	drawing(minirt);
	return (0);
}
