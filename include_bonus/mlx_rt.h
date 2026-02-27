/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_rt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 01:15:42 by tlavared          #+#    #+#             */
/*   Updated: 2026/01/31 02:04:03 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_RT_H
# define MLX_RT_H

#include "miniRt.h"

// src/draw/draw.c
int		draw(t_minirt *minirt);
t_color	ray_color(t_ray *r, t_scene *scene);
t_color	sky_color(t_ray *r);

// src/draw/draw_utils.c
t_color	pixel_color_aa(t_minirt *minirt, int x, int y);

// src/mlx_utils.c
void	put(t_minirt *minirt, int x, int y, uint32_t color);
int		ft_errorinit(mlx_t *mlx);
int		ft_errorimg(mlx_t *mlx, mlx_image_t *img);
void	ft_clearimg(t_minirt *minirt);

#endif
