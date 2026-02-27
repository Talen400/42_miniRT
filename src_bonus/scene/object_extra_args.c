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

#include "../../MLX42/include/lodepng/lodepng.h"
#include "miniRt.h"
#include "parser.h"
#include "color.h"

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

static bool	set_checker(t_object *obj, char **tokens, int base_count,
	bool has_extra_args)
{
	double	scale;

	if (has_extra_args)
	{
		if (!parse_color(tokens[base_count + 5], &obj->color2))
			return (false);
		scale = ft_atod(tokens[base_count + 6]);
		obj->checker_scale = scale;
		obj->has_checker = true;
	}
	else
	{
		obj->color2 = init_color(CHECKER_COLOR2_DEFAULT);
		obj->checker_scale = CHECKER_SCALE_DEFAULT;
		obj->has_checker = false;
	}
	return (true);
}

static bool load_bump_map(t_object *obj, char *path)
{
	unsigned int	w;
	unsigned int	h;

	obj->bump_map = malloc (sizeof(t_bump_map));
	if (!obj->bump_map)
		return (false);
	if (lodepng_decode32_file(&obj->bump_map->data, &w, &h, path) != 0
		|| w == 0 || h == 0)
	{
		free(obj->bump_map);
		obj->bump_map = NULL;
		return (false);
	}
	obj->bump_map->width = w;
	obj->bump_map->height = h;
	return (true);
}

static bool	set_bump(t_object *obj, char **tokens, int base_count,
	bool has_extra_args)
{
	double			bump;


	if (has_extra_args)
	{
		bump = ft_atod(tokens[base_count + 7]);
		if (bump < 0.0)
			return (false);
		obj->bump_scale = bump;
		obj->bump_map = NULL;
		if (bump > 0.0 && ft_strcmp(tokens[base_count + 8], "none") != 0
			&& !load_bump_map(obj, tokens[base_count + 8]))
			return (false);
	}
	else
	{
		obj->bump_scale = BUMP_SCALE_DEFAULT;
		obj->bump_map = NULL;
	}
	return (true);
}

bool	set_extra_args(t_object *obj, char **tokens, int base_count)
{
	bool	has_extra_args;
	size_t	array_size;

	array_size = ft_array_size((void **)tokens);
	has_extra_args = (array_size == (size_t)base_count + B_NARGS);
	if (!set_reflectivity(obj, tokens, base_count, has_extra_args))
		return (false);
	if (!set_phong_params(obj, tokens, base_count, has_extra_args))
		return (false);
	if (!set_checker(obj, tokens, base_count, has_extra_args))
		return (false);
	if (!set_bump(obj, tokens, base_count, has_extra_args))
		return (false);
	return (true);
}
