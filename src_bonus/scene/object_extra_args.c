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

bool	set_specular(t_object *obj, char **tokens, int base_count)
{
	double	ks;
	double	shininess;
    int     size;

    size = ft_array_size((void **)tokens);
    if (size == base_count + 3)
    {
        ks = ft_atod(tokens[base_count]);
		if (ks < 0.0 || ks > 1)
			return (false);
		obj->ks = ks;
        shininess = ft_atod(tokens[base_count + 1]);
		if (shininess < 1 || shininess > 500)
			return (false);
		obj->shininess = shininess;
    }
    else
    {
        obj->ks = KS_DEFAULT;
        obj->shininess = SHININESS_DEFAULT;
    }
	return (true);
}

bool set_reflectivity(t_object *obj, char **tokens, int base_count)
{
    double  reflectivity;
    int     size;

    size = ft_array_size((void **)tokens);
    if (size == base_count + 3)
    {
        reflectivity = ft_atod(tokens[base_count + 2]);
        if (reflectivity < 0 || reflectivity > 1)
            return (false);
        obj->reflectivity = reflectivity;
    }
    else
        obj->reflectivity = REFLECTIVITY_DEFAULT;
    return (true);
}

bool set_extra_args(t_object *obj, char **tokens, int base_count)
{
    if(!set_reflectivity(obj, tokens, base_count))
        return (false);
    if (!set_specular(obj,tokens,base_count))
        return (false);
    return (true);
}