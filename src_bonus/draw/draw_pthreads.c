/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pthreads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 01:44:49 by tlavared          #+#    #+#             */
/*   Updated: 2026/03/04 20:59:47 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/miniRt.h"
#include "../include_bonus/mlx_rt.h"
#include "../include_bonus/color.h"
#include "../include_bonus/intersect.h"
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

static void	render_tile(t_tile	*tile, t_thread_data *d)
{
	int		y;
	int		x;
	t_color	color;

	y = tile->y_start;
	while (y < tile->y_end)
	{
		x = tile->x_start;
		while (x < tile->x_end)
		{
			color = pixel_color_aa(d->minirt, x, y);
			put(d->minirt, x, y, color_to_int32(color));
			x++;
		}
		y++;
	}
}

/*
static void	render_tile(t_tile *tile, t_thread_data *d)
{
	int			y;
	int			x;
	long long	start, end, duration;
	t_color		heat_color;

	start = get_time_us();
	y = tile->y_start;
	while (y < tile->y_end)
	{
		x = tile->x_start;
		while (x < tile->x_end)
		{
			pixel_color_aa(d->minirt, x, y);
			x++;
		}
		y++;
	}
	end = get_time_us();
	duration = end - start;
	heat_color = get_heat_color(duration, 100000);
	y = tile->y_start;
	while (y < tile->y_end)
	{
		x = tile->x_start;
		while (x < tile->x_end)
		{
			put(d->minirt, x, y, color_to_int32(heat_color));
			x++;
		}
		y++;
	}
}
*/

static void	*render_thread(void *arg)
{
	t_thread_data	*d;
	t_tiles_queue	*queue;
	int				tile_to_render;

	d = (t_thread_data *) arg;
	queue = &d->minirt->mlx.tiles_queue;
	while (1)
	{
		pthread_mutex_lock(&queue->mutex);
		tile_to_render = queue->idx;
		queue->idx++;
		pthread_mutex_unlock(&queue->mutex);
		if (tile_to_render >= queue->count)
			break ;
		render_tile(&queue->tiles[tile_to_render], d);
	}
	return (NULL);
}

static t_tile	*alloc_tiles(t_minirt *minirt)
{
	t_tile			*tiles;
	int				tiles_ver;
	int				tiles_hor;
	int				tiles_total;

	tiles_hor = (minirt->scene.width + TILES_SIZE - 1)/ TILES_SIZE;
	tiles_ver = (minirt->scene.height + TILES_SIZE - 1)/ TILES_SIZE;
	tiles_total = tiles_ver * tiles_hor;
	tiles = malloc (sizeof (t_tile) * tiles_total);
	if (!tiles)
		return (NULL);
	return (tiles);
}

void	init_tiles(t_minirt	*minirt)
{
	t_tiles_queue	*queue;
	int	x;
	int	y;
	int	i;

	printf("antes de pega e atribuir a variavel queue \n");
	queue = &minirt->mlx.tiles_queue;
	queue->tiles = alloc_tiles(minirt);
	printf("peguei a variavel e vamos atribuir \n");
	queue->count = 0;
	queue->idx = 0;
	if (!queue->tiles)
		return ;
	pthread_mutex_init(&queue->mutex, NULL);
	i = 0;
	y = 0;
	while (y < minirt->scene.height)
	{
		x = 0;
		while (x < minirt->scene.width)
		{
			queue->tiles[i].x_start = x;
			queue->tiles[i].y_start = y;
			if (x + TILES_SIZE > minirt->scene.width)
				queue->tiles[i].x_end = WIDTH;
			else
				queue->tiles[i].x_end = x + TILES_SIZE;
			if (y + TILES_SIZE > minirt->scene.height)
				queue->tiles[i].y_start = HEIGHT;
			else
				queue->tiles[i].y_end = y + TILES_SIZE;
			i++;
			x += TILES_SIZE;
		}
		y += TILES_SIZE;
	}
	queue->count = i;
}

static void	drawing(t_minirt *minirt)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	data[NUM_THREADS];
	int				i;

	printf("antes do allocamento \n");
	init_tiles(minirt);
	printf("depois do alocamento \n");
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
}

int	draw(t_minirt *minirt)
{
	ft_clearimg(minirt);
	drawing(minirt);
	return (0);
}
