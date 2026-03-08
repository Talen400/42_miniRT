#include "../include_bonus/miniRt.h"
#include "../include_bonus/mlx_rt.h"
#include "../include_bonus/color.h"
#include "../include_bonus/intersect.h"


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

   To debug threads in heat color:
*/
/*
void	render_tile(t_tile *tile, t_thread_data *d)
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
	// ajust time
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
void	*render_thread(void *arg)
{
	t_thread_data	*d;
	t_tiles_queue	*queue;
	int				tile_to_render;

	d = (t_thread_data *) arg;
	queue = &d->minirt->mlx.tiles_queue;
	while (1)
	{
		tile_to_render = queue->idx;		queue->idx++;
		if (tile_to_render >= queue->count)
			break ;
		render_tile(&queue->tiles[tile_to_render], d);
	}
	return (NULL);
}

