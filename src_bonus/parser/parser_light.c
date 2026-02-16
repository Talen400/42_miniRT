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

static void	add_light_to_scene(t_scene *scene, t_light *new_light)
{
	t_light	*current;

	if (!scene->lights)
	{
		scene->lights = new_light;
		return ;
	}
	current = scene->lights;
	while (current->next)
		current = current->next;
	current->next = new_light;
}

static bool	validate_light(char **tokens, t_parse_context *ctx,
							t_light_data *data)
{
	if (ft_array_size((void **)tokens) != 4)
	{
		ft_error_and_free(ctx, tokens,
			"light: expected format 'L <X,Y,Z> <ratio> <R,G,B>'");
		return (false);
	}
	if (!parse_vector(tokens[1], &data->position))
	{
		ft_error_and_free(ctx, tokens, "light: invalid position format");
		return (false);
	}
	data->brightness = ft_atod(tokens[2]);
	if (!validate_ratio(data->brightness))
	{
		ft_error_and_free(ctx, tokens,
			"light intensity must be in range [0.0, 1.0]");
		return (false);
	}
	if (!parse_color(tokens[3], &data->color))
	{
		ft_error_and_free(ctx, tokens, "light: invalid color format");
		return (false);
	}
	return (true);
}

static t_light	*light_creat(t_light_data *data)
{
	t_light	*light;

	light = (t_light *)malloc(sizeof(t_light));
	if (!light)
		return (NULL);
	light->position = data->position;
	light->brightness = data->brightness;
	light->color = data->color;
	light->next = NULL;
	return (light);
}

bool	parse_light(t_parse_context *ctx, t_scene *scene, const char *line)
{
	char			**tokens;
	t_light_data	data;
	t_light			*light;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (false);
	if (!validate_light(tokens, ctx, &data))
		return (false);
	light = light_creat(&data);
	if (!light)
	{
		ft_error_and_free(ctx, tokens, "light: memory allocation failed");
		return (false);
	}
	ctx->light_count++;
	ft_free_split(tokens);
	add_light_to_scene(scene, light);
	return (true);
}
