/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:15:21 by tlavared          #+#    #+#             */
/*   Updated: 2026/02/02 17:16:41 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRt.h"

t_ray	ray_create(t_vec3 origin, t_vec3 direction)
{
	t_ray	r;

	r.origin = origin;
	r.direction = direction;
	return (r);
}

/*
 * Need normalize the direction before calculation ray
 */

t_vec3	ray_at(t_ray r, double t)
{
	return (vec3_add(r.origin, vec3_multiply(r.direction, t)));
}
