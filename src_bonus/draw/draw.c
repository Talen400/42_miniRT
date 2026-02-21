/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 01:44:49 by tlavared          #+#    #+#             */
/*   Updated: 2026/02/21 19:38:21 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/miniRt.h"
#include "mlx_rt.h"
#include "color.h"
#include "intersect.h"

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

static t_ray	pixel_ray(t_camera *cam, int x, int y, t_scene *scene)
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

static void	drawing(t_minirt *minirt)
{
	int		x;
	int		y;
	t_ray	r;
	t_color	color;

	y = 0;
	while (y < minirt->scene.height)
	{
		x = 0;
		while (x < minirt->scene.width)
		{
			r = pixel_ray(&minirt->scene.camera, x, y, &minirt->scene);
			color = ray_color(&r, &minirt->scene);
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
