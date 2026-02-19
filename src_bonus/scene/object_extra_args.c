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

#include "miniRt.h"
#include "parser.h"

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

static bool	set_phong_params(t_object *obj, char **tokens, int base_count,
	bool has_extra_args)
{
	if (has_extra_args)
	{
		obj->ks = ft_atod(tokens[base_count]);
		obj->kd = ft_atod(tokens[base_count + 1]);
		obj->ka = ft_atod(tokens[base_count + 2]);
		obj->shininess = ft_atod(tokens[base_count + 3]);
		if (!validate_phong_params(obj))
			return (false);
	}
	else
	{
		obj->ks = KS_DEFAULT;
		obj->kd = KD_DEFAULT;
		obj->ka = KA_DEFAULT;
		obj->shininess = SHININESS_DEFAULT;
	}
	return (true);
}

static bool	set_reflectivity(t_object *obj, char **tokens, int base_count,
		bool has_extra_args)
{
	double	reflectivity;

	if (has_extra_args)
	{
		reflectivity = ft_atod(tokens[base_count + 4]);
		if (reflectivity < 0 || reflectivity > 1)
			return (false);
		obj->reflectivity = reflectivity;
	}
	else
		obj->reflectivity = REFLECTIVITY_DEFAULT;
	return (true);
}

bool	set_extra_args(t_object *obj, char **tokens, int base_count)
{
	bool	has_extra_args;

	has_extra_args = (ft_array_size((void **)tokens) == (size_t)base_count + 5);
	if (!set_reflectivity(obj, tokens, base_count, has_extra_args))
		return (false);
	if (!set_phong_params(obj, tokens, base_count, has_extra_args))
		return (false);
	return (true);
}
