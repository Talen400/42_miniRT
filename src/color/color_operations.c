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

t_color	color_add(t_color a, t_color b)
{
	t_color	result;

	result.r = a.r + b.r;
	result.g = a.g + b.g;
	result.b = a.b + b.b;
	result.a = a.a + b.a;
	if (result.r > 255)
		result.r = 255;
	if (result.g > 255)
		result.g = 255;
	if (result.b > 255)
		result.b = 255;
	if (result.a > 255)
		result.a = 255;
	return (result);
}

t_color	color_subtract(t_color a, t_color b)
{
	t_color	result;

	result.r = a.r - b.r;
	result.g = a.g - b.g;
	result.b = a.b - b.b;
	result.a = a.a - b.a;
	if (result.r < 0)
		result.r = 0;
	if (result.g < 0)
		result.g = 0;
	if (result.b < 0)
		result.b = 0;
	if (result.a < 0)
		result.a = 0;
	return (result);
}

t_color	color_scale(t_color c, double t)
{
	t_color	result;

	result.r = (uint8_t)(c.r * t);
	result.g = (uint8_t)(c.g * t);
	result.b = (uint8_t)(c.b * t);
	result.a = c.a;
	if (result.r > 255)
		result.r = 255;
	if (result.g > 255)
		result.g = 255;
	if (result.b > 255)
		result.b = 255;
	if (result.a > 255)
		result.a = 255;
	return (result);
}
