/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_plane.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-06 11:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-06 11:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"

static bool	validate_normal_vector(t_vec3 *normal, t_parse_context *ctx,
								char **tokens)
{
	double	length;

	length = vec3_length(*normal);
	if (length < 1e-6)
	{
		ft_error_and_free(ctx, tokens,
			"Plane: normal vector cannot be zero");
		return (false);
	}
	*normal = vec3_normalize(*normal);
	return (true);
}

static bool	validate_plane(char **tokens, t_parse_context *ctx,
							t_plane_data *data)
{
	if (ft_array_size((void **)tokens) != PL_NARGS &&
		ft_array_size((void **)tokens) != PL_NARGS + B_NARGS)
	{
		ft_error_and_free(ctx, tokens,
			"Plane: expected format 'pl <x,y,z> <nx,ny,nz> <R,G,B>'");
		return (false);
	}
	if (!parse_vector(tokens[1], &data->point))
	{
		ft_error_and_free(ctx, tokens, "Plane: invalid point format");
		return (false);
	}
	if (!parse_vector(tokens[2], &data->normal))
	{
		ft_error_and_free(ctx, tokens, "Plane: invalid normal vector format");
		return (false);
	}
	if (!validate_normal_vector(&data->normal, ctx, tokens))
		return (false);
	if (!parse_color(tokens[3], &data->color))
	{
		ft_error_and_free(ctx, tokens, "Plane: invalid color format");
		return (false);
	}
	return (true);
}

static t_plane	create_plane_data(t_plane_data *data)
{
	t_plane	plane;

	plane.point = data->point;
	plane.normal = data->normal;
	plane.color = data->color;
	return (plane);
}

bool	parse_plane(t_parse_context *ctx, t_scene *scene, const char *line)
{
	char			**tokens;
	t_plane_data	data;
	t_plane			plane;
	t_object		*obj;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (false);
	if (!validate_plane(tokens, ctx, &data))
		return (false);
	plane = create_plane_data(&data);
	obj = create_plane_obj(plane, tokens);
	if (!obj)
	{
		ft_error_and_free(ctx, tokens, "Plane: failed to create object");
		return (false);
	}
	add_object_to_scene(scene, obj);
	ft_free_split(tokens);
	return (true);
}
