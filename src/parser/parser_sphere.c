/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_light.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-05 18:40:16 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-05 18:40:16 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"

static bool	validate_sphere(char **tokens, t_parse_context *ctx,
							t_sphere_data *data)
{
	if (ft_array_size((void **)tokens) != 4)
	{
		ft_error_and_free(ctx, tokens,
			"sphere: expected format 'sp <X,Y,Z> <diameter> <R,G,B>'");
		return (false);
	}
	if (!parse_vector(tokens[1], &data->center))
	{
		ft_error_and_free(ctx, tokens, " sphere: invalid position format");
		return (false);
	}
	data->diameter = ft_atod(tokens[2]);
	if (data->diameter <= 0)
	{
		ft_error_and_free(ctx, tokens, "sphere diameter must be > 0");
		return (false);
	}
	if (!parse_color(tokens[3], &data->color))
	{
		ft_error_and_free(ctx, tokens, "sphere: invalid color format");
		return (false);
	}
	return (true);
}

static t_sphere	sphere_creat(t_sphere_data *data)
{
	t_sphere	sphere;

	sphere.center = data->center;
	sphere.diameter = data->diameter;
	sphere.radius = data->diameter / 2.0;
	sphere.color = data->color;
	return (sphere);
}

bool	parse_sphere(t_parse_context *ctx, t_scene *scene, const char *line)
{
	char			**tokens;
	t_sphere_data	data;
	t_sphere		sphere;
	t_object		*obj;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (false);
	if (!validate_sphere(tokens, ctx, &data))
		return (false);
	sphere = sphere_creat(&data);
	obj = create_sphere_obj(sphere);
	if (!obj)
	{
		ft_error_and_free(ctx, tokens, "Sphere: failed to create object");
		return (false);
	}
	ft_free_split(tokens);
	add_object_to_scene(scene, obj);
	return (true);
}
