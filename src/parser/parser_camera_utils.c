/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_camera_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-11 14:50:45 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-11 14:50:45 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"
#include <math.h>

void	calculate_viewport(t_camera *camera)
{
	double	theta;
	double	h;
	t_vec3	horizontal;
	t_vec3	vertical;

	theta = camera->fov * M_PI / 180.0;
	h = tan(theta / 2.0);
	camera->viewport_height = 2.0 * h;
	camera->viewport_width = camera->viewport_height
		* (WIDTH / (double)HEIGHT);
	horizontal = vec3_multiply(camera->right, camera->viewport_width);
	vertical = vec3_multiply(camera->up, camera->viewport_height);
	vertical.y *= -1;
	camera->lower_left_corner = camera->position;
	camera->lower_left_corner = vec3_add(camera->lower_left_corner,
			camera->forward);
	camera->lower_left_corner = vec3_subtract(camera->lower_left_corner,
			vec3_divide(horizontal, 2.0));
	camera->lower_left_corner = vec3_subtract(camera->lower_left_corner,
			vec3_divide(vertical, 2.0));
	camera->horizontal = horizontal;
	camera->vertical = vertical;
}
