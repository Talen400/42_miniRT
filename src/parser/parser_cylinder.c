/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cylinder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-06 12:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-06 12:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"

static bool	validate_and_normalize_axis(t_vec3 *axis, t_parse_context *ctx,
										char **tokens)
{
	double	length;

	length = vec3_length(*axis);
	if (length < 1e-6)
	{
		ft_error_and_free(ctx, tokens,
			"Cylinder: axis vector cannot be zero");
		return (false);
	}
	*axis = vec3_normalize(*axis);
	return (true);
}

static bool helper_validate_cylinder(char **tokens, t_parse_context *ctx,
								t_cylinder_data *data)
{
    if (!validate_and_normalize_axis(&data->axis, ctx, tokens))
		return (false);
	data->diameter = ft_atod(tokens[3]);
	if (data->diameter <= 0.0)
	{
		ft_error_and_free(ctx, tokens, "Cylinder: diameter must be > 0");
		return (false);
	}
	data->height = ft_atod(tokens[4]);
	if (data->height <= 0.0)
	{
		ft_error_and_free(ctx, tokens, "Cylinder: height must be > 0");
		return (false);
	}
	if (!parse_color(tokens[5], &data->color))
	{
		ft_error_and_free(ctx, tokens, "Cylinder: invalid color format");
		return (false);
	}
    return (true);
}
static bool	validate_cylinder(char **tokens, t_parse_context *ctx,
								t_cylinder_data *data)
{
	if (ft_array_size((void **)tokens) != 6)
	{
		ft_error_and_free(ctx, tokens,
			"Cylinder: expected format 'cy <x,y,z> <nx,ny,nz> <d> <h> <R,G,B>'");
		return (false);
	}
	if (!parse_vector(tokens[1], &data->center))
	{
		ft_error_and_free(ctx, tokens, "Cylinder: invalid center format");
		return (false);
	}
	if (!parse_vector(tokens[2], &data->axis))
	{
		ft_error_and_free(ctx, tokens, "Cylinder: invalid axis format");
		return (false);
	}
	if (!helper_validate_cylinder(tokens, ctx, data))
        return (false);
	return (true);
}

static t_cylinder	create_cylinder_data(t_cylinder_data *data)
{
	t_cylinder	cyl;

	cyl.center = data->center;
	cyl.axis = data->axis;
	cyl.diameter = data->diameter;
	cyl.height = data->height;
	cyl.color = data->color;
	cyl.radius = data->diameter / 2.0;
	return (cyl);
}

bool	parse_cylinder(t_parse_context *ctx, t_scene *scene, const char *line)
{
	char			**tokens;
	t_cylinder_data	data;
	t_cylinder		cylinder;
	t_object		*obj;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (false);
	if (!validate_cylinder(tokens, ctx, &data))
		return (false);
	cylinder = create_cylinder_data(&data);
	obj = create_cylinder_obj(cylinder);
	if (!obj)
	{
		ft_error_and_free(ctx, tokens, "Cylinder: failed to create object");
		return (false);
	}
	add_object_to_scene(scene, obj);
	ft_free_split(tokens);
	return (true);
}
