/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ambient.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-05 16:55:53 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-05 16:55:53 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"

bool	validate_ratio(double ratio)
{
	return (ratio >= 0.0 && ratio <= 1.0);
}

static bool	validate_ambient(char **tokens, t_parse_context *ctx,
							double *intensity, t_color *color)
{
	if (ft_array_size((void **)tokens) != 3)
	{
		ft_error_and_free(ctx, tokens,
			"Ambient light: expected format 'A <ratio> <R,G,B>'");
		return (false);
	}
	*intensity = ft_atod(tokens[1]);
	if (!validate_ratio(*intensity))
	{
		ft_error_and_free(ctx, tokens,
			"Ambient light intensity must be in range [0.0, 1.0]");
		return (false);
	}
	if (!parse_color(tokens[2], color))
	{
		ft_error_and_free(ctx, tokens,
			"Ambient light: invalid color format");
		return (false);
	}
	return (true);
}

bool	parse_ambient(t_parse_context *ctx, t_scene *scene, const char *line)
{
	char	**tokens;
	double	intensity;
	t_color	color;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (false);
	if (!validate_ambient(tokens, ctx, &intensity, &color))
		return (false);
	scene->ambient.intensity = intensity;
	scene->ambient.color = color;
	ctx->ambient_count++;
	ft_free_split(tokens);
	return (true);
}
