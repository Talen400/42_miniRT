/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-19 17:19:09 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-19 17:19:09 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniRt.h"
#include "parser.h"

static bool	helper_validate_cone(char **tokens, t_parse_context *ctx,
								t_cone_data *data)
{
	if (!validate_and_normalize_axis(&data->axis, ctx, tokens))
		return (false);
	data->diameter = ft_atod(tokens[3]);
	if (data->diameter <= 0.0)
	{
		ft_error_and_free(ctx, tokens, "Cone: diameter must be > 0");
		return (false);
	}
	data->height = ft_atod(tokens[4]);
	if (data->height <= 0.0)
	{
		ft_error_and_free(ctx, tokens, "Cone: height must be > 0");
		return (false);
	}
	if (!parse_color(tokens[5], &data->color))
	{
		ft_error_and_free(ctx, tokens, "Cone: invalid color format");
		return (false);
	}
	return (true);
}

static bool	validate_cone(char **tokens, t_parse_context *ctx,
								t_cone_data *data)
{
	if (ft_array_size((void **)tokens) != CN_NARGS &&
		ft_array_size((void **)tokens) != CN_NARGS + B_NARGS)
	{
		ft_error_and_free(ctx, tokens,
			"Cone: format 'cn <x,y,z> <nx,ny,nz> <d> <h> <R,G,B>'");
		return (false);
	}
	if (!parse_vector(tokens[1], &data->apex))
	{
		ft_error_and_free(ctx, tokens, "Cone: invalid apex format");
		return (false);
	}
	if (!parse_vector(tokens[2], &data->axis))
	{
		ft_error_and_free(ctx, tokens, "Cone: invalid axis format");
		return (false);
	}
	if (!helper_validate_cone(tokens, ctx, data))
		return (false);
	return (true);
}

static t_cone	create_cone_data(t_cone_data *data)
{
	t_cone cn;

	cn.apex = data->apex;
	cn.axis = data->axis;
	cn.diameter = data->diameter;
	cn.radius = data->diameter / 2;
	cn.height = data->height;
	cn.half_angle = cn.radius / cn.height;
	cn.color = data->color;
	return (cn);
}

bool	parse_cone(t_parse_context *ctx, t_scene *scene, const char *line)
{
	char			**tokens;
	t_cone_data		data;
	t_cone			cone;
	t_object		*obj;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (false);
	if (!validate_cone(tokens, ctx, &data))
		return (false);
	cone = create_cone_data(&data);
	obj = create_cone_obj(cone, tokens);
	if (!obj)
	{
		ft_error_and_free(ctx, tokens, "Cone: failed to create object");
		return (false);
	}
	add_object_to_scene(scene, obj);
	ft_free_split(tokens);
	return (true);
}
