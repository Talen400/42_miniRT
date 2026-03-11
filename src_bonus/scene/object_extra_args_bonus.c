/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_extra_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 15:07:34 by rgregori          #+#    #+#             */
/*   Updated: 2026-01-29 15:07:34 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt_bonus.h"
#include "parser_bonus.h"

static bool	validate_phong_params(t_object *obj)
{
	if (obj->ks < 0.0 || obj->ks > 1)
		return (false);
	if (obj->ka < 0.0 || obj->ka > 1)
		return (false);
	if (obj->kd < 0.0 || obj->kd > 1)
		return (false);
	if (obj->shininess < 1 || obj->shininess > 500)
		return (false);
	return (true);
}

static bool	set_checker(t_object *obj, char **tokens, int base_count,
				int extra)
{
	double	scale;

	if (extra < B_CHECKER_NARGS)
		return (extra == B_REFL_NARGS);
	if (!parse_color(tokens[base_count + 5], &obj->color2))
		return (false);
	scale = ft_atod(tokens[base_count + 6]);
	obj->checker_scale = scale;
	obj->has_checker = true;
	return (set_bump(obj, tokens, base_count, extra));
}

static bool	set_reflectivity(t_object *obj, char **tokens, int base_count,
				int extra)
{
	double	reflectivity;

	if (extra < B_REFL_NARGS)
		return (extra == B_PHONG_NARGS);
	reflectivity = ft_atod(tokens[base_count + 4]);
	if (reflectivity < 0 || reflectivity > 1)
		return (false);
	obj->reflectivity = reflectivity;
	return (set_checker(obj, tokens, base_count, extra));
}

static bool	set_phong_params(t_object *obj, char **tokens, int base_count,
	int extra)
{
	if (extra < B_PHONG_NARGS)
		return (extra == 0);
	obj->ks = ft_atod(tokens[base_count]);
	obj->kd = ft_atod(tokens[base_count + 1]);
	obj->ka = ft_atod(tokens[base_count + 2]);
	obj->shininess = ft_atod(tokens[base_count + 3]);
	if (!validate_phong_params(obj))
		return (false);
	return (set_reflectivity(obj, tokens, base_count, extra));
}

bool	set_extra_args(t_object *obj, char **tokens, int base_count)
{
	int		extra;
	size_t	array_size;

	array_size = ft_array_size((void **)tokens);
	extra = (int)array_size - base_count;
	return (set_phong_params(obj, tokens, base_count, extra));
}
