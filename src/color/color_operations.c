/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:56:09 by tlavared          #+#    #+#             */
/*   Updated: 2026/02/02 16:14:54 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/color.h"
#include "miniRt.h"

static uint8_t	clamp_channel(int value)
{
	if (value > 255)
		return (255);
	if (value < 0)
		return (0);
	return ((uint8_t)value);
}

t_color	color_add(t_color a, t_color b)
{
	t_color	result;

	result.r = clamp_channel((int)a.r + (int)b.r);
	result.g = clamp_channel((int)a.g + (int)b.g);
	result.b = clamp_channel((int)a.b + (int)b.b);
	result.a = 255;
	return (result);
}

t_color	color_subtract(t_color a, t_color b)
{
	t_color	result;

	result.r = clamp_channel((int)a.r - (int)b.r);
	result.g = clamp_channel((int)a.g - (int)b.g);
	result.b = clamp_channel((int)a.b - (int)b.b);
	result.a = clamp_channel((int)a.a - (int)b.a);
	return (result);
}

t_color	color_scale(t_color c, double t)
{
	t_color	result;

	result.r = clamp_channel((int)(c.r * t));
	result.g = clamp_channel((int)(c.g * t));
	result.b = clamp_channel((int)(c.b * t));
	result.a = c.a;
	return (result);
}

t_color	color_clamp(t_color c)
{
	c.r = clamp_channel(c.r);
	c.g = clamp_channel(c.g);
	c.b = clamp_channel(c.b);
	c.a = 255;
	return (c);
}
