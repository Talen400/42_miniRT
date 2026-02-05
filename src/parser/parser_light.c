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

bool	add_light_to_scene(t_scene *scene, t_light *light)
{

}

static bool	validate_light(char **tokens, t_parse_context *ctx,
							double *intensity, t_color *color)
{
	if (ft_array_size((void **)tokens) != 4)
	{
		ft_error_and_free(ctx, tokens,
			"Ambient light: expected format 'L <X,Y,Z> <ratio> <R,G,B>'");
		return (false);
	}
	*intensity = ft_atod(tokens[2]);
	if (!validate_ratio(*intensity))
	{
		ft_error_and_free(ctx, tokens,
			"Ambient light intensity must be in range [0.0, 1.0]");
		return (false);
	}
	if (!parse_color(tokens[3], color))
	{
		ft_error_and_free(ctx, tokens,
			"Ambient light: invalid color format");
		return (false);
	}
	return (true);
}

static t_light *light_creat(double intensity, t_color color, t_vec3 vec)
{
	t_light	light;

	light = (t_light *)malloc(sizeof(t_light));
	if (!light)
	{
		ft_error_and_free(ctx, tokens,
			"light: memory allocation failed");
		return (NULL);
	}
	light.intensity = intensity;
	light.color = color;
	light.position = vec;
	return (light);
}

bool	parse_light(t_parse_context *ctx, t_scene *scene, const char *line)
{
	char		**tokens;
	double		intensity;
	t_color		color;
	t_vec3		vec;
	t_light		*light;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (false);
	if (!validate_light((const char **)tokens, ctx, &intensity, &color))
		return (false);
	if (!parse_vector(tokens[2],vec))
	{
		ft_error_and_free(ctx, tokens,
			"light: invalid vector format");
		return (false);
	}
	light = light_create(intensity, color, vec);
	if (!light)
		return (false);
	ctx->light_count++;
	ft_free_split(tokens);
	add_light_to_scene(scene, light);
	return (true);
}
