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
int	draw(t_minirt *minirt);

// src/mlx_utils.c
int	ft_errorinit(mlx_t *mlx);
int	ft_errorimg(mlx_t *mlx, mlx_image_t *img);
void	ft_clearimg(uint8_t *pixels);

#endif
