/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pthreads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 01:44:49 by tlavared          #+#    #+#             */
/*   Updated: 2026/03/09 18:48:08 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt_bonus.h"
#include "mlx_rt_bonus.h"
#include "color_bonus.h"
#include "intersect_bonus.h"
#include <stdlib.h>

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
	pthread_t		threads[NUM_THREADS];
	t_thread_data	data[NUM_THREADS];
	int				i;

	init_tiles(minirt);
	i = 0;
	while (i < NUM_THREADS)
	{
		data[i].id = i;
		data[i].minirt = minirt;
		pthread_create(&threads[i], NULL, render_thread, &data[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);
	destroy_tiles(&minirt->mlx.tiles_queue);
}

int	draw(t_minirt *minirt)
{
	pthread_mutex_lock(&minirt->mlx.render_mutex);
	ft_clearimg(minirt);
	drawing(minirt);
	pthread_mutex_unlock(&minirt->mlx.render_mutex);
	ft_putstr_fd("rendered!\n", 1);
	return (0);
}
