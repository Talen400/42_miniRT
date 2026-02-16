/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:45:27 by tlavared          #+#    #+#             */
/*   Updated: 2026/02/02 16:23:48 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

#include "stdint.h"

typedef struct	s_color
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;
} t_color;

// color_convert.c
t_color		init_color(uint32_t color);
uint32_t	color_to_int32(t_color color);

// color_operations.c
t_color		color_add(t_color a, t_color b);
t_color		color_subtract(t_color a, t_color b);
t_color		color_scale(t_color c, double t);
t_color		color_clamp(t_color c);

#endif
