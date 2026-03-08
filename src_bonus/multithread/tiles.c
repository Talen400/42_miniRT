#include "../../include_bonus/miniRt.h"

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

	queue = &minirt->mlx.tiles_queue;
	queue->tiles = alloc_tiles(minirt);
	queue->count = 0;
	queue->idx = 0;
	if (!queue->tiles)
		return ;
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

