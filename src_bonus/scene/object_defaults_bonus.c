/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_defaults.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-04 17:15:39 by rgregori          #+#    #+#             */
/*   Updated: 2026-03-04 17:15:39 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt_bonus.h"
#include "parser_bonus.h"
#include "color_bonus.h"

void	init_object_defaults(t_object *obj)
{
	obj->ks = KS_DEFAULT;
	obj->kd = KD_DEFAULT;
	obj->ka = KA_DEFAULT;
	obj->shininess = SHININESS_DEFAULT;
	obj->reflectivity = REFLECTIVITY_DEFAULT;
	obj->color2 = init_color(CHECKER_COLOR2_DEFAULT);
	obj->checker_scale = CHECKER_SCALE_DEFAULT;
	obj->has_checker = false;
	obj->bump_scale = BUMP_SCALE_DEFAULT;
	obj->bump_map = NULL;
}
