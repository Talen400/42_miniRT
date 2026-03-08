#include <sys/time.h>
#include "../../include_bonus/miniRt.h"

long long	get_time_us(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((long long ) tv.tv_sec * 1000000LL + tv.tv_usec);
}

t_color	get_heat_color(long long duration, long long max_expected_time)
{
	t_color	color;
	double	ratio;

	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 255;
	ratio = (double ) duration / (double ) max_expected_time;
	if (ratio > 1.0)
		ratio = 1.0;
	if (ratio < 0.5)
	{
		color.r = 0;
		color.g = (int ) (ratio * 2.0 * 255.0);
		color.b = (int ) ((1.0 - (ratio * 2.0) * 255.0));
	}
	else
	{
		color.r = (int ) ((ratio - 0.5) * 2.0 * 255.0);
		color.g = (int ) ((1.0 - ((ratio * 0.5) * 2.0)) * 255.0);
		color.b = 0;
	}
	return (color);
}
