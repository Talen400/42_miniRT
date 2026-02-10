/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 17:40:19 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/02 13:20:22 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"
#include "scene.h"

void    init_camera(t_camera *cam)
{
    double  aspect_ratio;
    double  half_width;
    double  half_height;
    t_vec3  world_up;

    cam->forward = vec3_normalize(cam->orientation);
    world_up = vec3_create(0, 1, 0);
    if (fabs(cam->forward.y) > 0.999)
        world_up = vec3_create(1, 0, 0);
    cam->right = vec3_normalize(vec3_cross(cam->forward, world_up));
    cam->up = vec3_normalize(vec3_cross(cam->right, cam->forward));
    aspect_ratio = (double)WIDTH / (double)HEIGHT;
    half_width = tan((cam->fov * PI / 180.0) / 2.0);
    half_height = half_width / aspect_ratio;
    cam->viewport_width = half_width * 2.0;
    cam->viewport_height = half_height * 2.0;
    cam->horizontal = vec3_multiply(cam->right, cam->viewport_width);
    cam->vertical = vec3_multiply(cam->up, cam->viewport_height);
    cam->lower_left_corner = vec3_add(cam->position,
        vec3_subtract(cam->forward,
            vec3_add(vec3_multiply(cam->horizontal, 0.5),
                     vec3_multiply(cam->vertical, 0.5))));
}
