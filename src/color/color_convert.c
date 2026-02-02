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
	return ((color.r << 24) | (color.g << 16) | (color.b << 8) | color.a);
}
