/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_bump.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-04 00:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026/03/04 18:35:12 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MLX42/include/lodepng/lodepng.h"
#include "../../include_bonus/miniRt.h"
#include "parser.h"

static bool	load_bump_map(t_object *obj, char *path)
{
	obj->bump_map = malloc(sizeof(t_bump_map));
	if (!obj->bump_map)
		return (false);
	if (lodepng_decode32_file(&obj->bump_map->data,
			&obj->bump_map->width, &obj->bump_map->height,
			path) != 0
		|| obj->bump_map->width == 0 || obj->bump_map->height == 0)
	{
		free(obj->bump_map);
		obj->bump_map = NULL;
		return (false);
	}
	return (true);
}

bool	set_bump(t_object *obj, char **tokens, int base_count, int extra)
{
	double	bump;

	if (extra < B_BUMP_NARGS)
		return (extra == B_CHECKER_NARGS);
	bump = ft_atod(tokens[base_count + 7]);
	if (bump < 0.0)
		return (false);
	obj->bump_scale = bump;
	if (bump > 0.0 && ft_strcmp(tokens[base_count + 8], "none") != 0)
		return (load_bump_map(obj, tokens[base_count + 8]));
	return (extra == B_BUMP_NARGS);
}
