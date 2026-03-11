/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tiles.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 18:38:04 by tlavared          #+#    #+#             */
/*   Updated: 2026/03/09 19:02:21 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt_bonus.h"

static t_tile	*alloc_tiles(t_minirt *minirt)
{
	t_tile	*tiles;
	int		tiles_ver;
	int		tiles_hor;
	int		tiles_total;

	tiles_hor = (minirt->scene.width + TILES_SIZE - 1) / TILES_SIZE;
	tiles_ver = (minirt->scene.height + TILES_SIZE - 1) / TILES_SIZE;
	tiles_total = tiles_ver * tiles_hor;
	tiles = malloc (sizeof (t_tile) * tiles_total);
	if (!tiles)
		return (NULL);
	return (tiles);
}

static void	init_tile_lst(t_minirt *minirt, t_tiles_queue *queue)
{
	int	x;
	int	y;

	y = 0;
	while (y < minirt->scene.height)
	{
		x = 0;
		while (x < minirt->scene.width)
		{
			queue->tiles[queue->count].x_start = x;
			queue->tiles[queue->count].y_start = y;
			if (x + TILES_SIZE > minirt->scene.width)
				queue->tiles[queue->count].x_end = minirt->scene.width;
			else
				queue->tiles[queue->count].x_end = x + TILES_SIZE;
			if (y + TILES_SIZE > minirt->scene.height)
				queue->tiles[queue->count].y_end = minirt->scene.height;
			else
				queue->tiles[queue->count].y_end = y + TILES_SIZE;
			queue->count++;
			x += TILES_SIZE;
		}
		y += TILES_SIZE;
	}
}

void	destroy_tiles(t_tiles_queue *queue)
{
	pthread_mutex_destroy(&queue->mutex);
	free(queue->tiles);
	queue->tiles = NULL;
}

void	init_tiles(t_minirt	*minirt)
{
	t_tiles_queue	*queue;

	queue = &minirt->mlx.tiles_queue;
	queue->tiles = alloc_tiles(minirt);
	queue->count = 0;
	queue->idx = 0;
	pthread_mutex_init(&queue->mutex, NULL);
	if (!queue->tiles)
		return ;
	init_tile_lst(minirt, queue);
}
