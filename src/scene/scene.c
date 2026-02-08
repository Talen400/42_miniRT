/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-05 17:08:32 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-05 17:08:32 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"

t_scene scene_init(void)
{
	t_scene scene;

	scene.camera = (t_camera){};
	scene.ambient = (t_ambient){};
	scene.lights = NULL;
	scene.objects = NULL;
	scene.width = WIDTH;
	scene.height = HEIGHT;
	return (scene);
}
