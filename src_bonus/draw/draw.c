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

#include "miniRt.h"
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

t_color	ray_color(t_ray *r, t_scene *scene)
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
