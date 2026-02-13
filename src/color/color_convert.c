/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_convert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:45:09 by tlavared          #+#    #+#             */
/*   Updated: 2026/02/02 16:24:12 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/color.h"
#include "miniRt.h"

t_color	init_color(uint32_t color)
{
	t_color	result;

	result.r = (color >> 24) & 0xFF;
	result.g = (color >> 16) & 0xFF;
	result.b = (color >> 8) & 0xFF;
	result.a = color & 0xFF;
	return (result);
}

uint32_t	color_to_int32(t_color color)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	// Clamp + cast para uint8_t
	r = (uint8_t)fmin(255.0, fmax(0.0, color.r));
	g = (uint8_t)fmin(255.0, fmax(0.0, color.g));
	b = (uint8_t)fmin(255.0, fmax(0.0, color.b));
	a = 255;
	
	return ((r << 24) | (g << 16) | (b << 8) | a);
}
