/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 15:07:38 by rgregori          #+#    #+#             */
/*   Updated: 2026-01-29 15:07:38 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

const t_element_parser	g_element_parsers[] = {
{"A", parse_ambient},
{"C", parse_camera},
{"L", parse_light},
{"sp", parse_sphere},
{"pl", parse_plane},
{"cy", parse_cylinder},
{"cn", parse_cone},
{NULL, NULL}
};

static t_parse_context	ft_init_parse_context(const char *filename)
{
	t_parse_context	context;

	context.camera_count = 0;
	context.ambient_count = 0;
	context.light_count = 0;
	context.current_line = 0;
	context.filename = filename;
	context.error_occurred = false;
	return (context);
}

static bool	validate_scene_counts(t_parse_context *ctx)
{
	if (ctx->camera_count != 1)
	{
		print_parse_error(ctx->filename, ctx->current_line,
			"Expected exactly 1 camera");
		return (false);
	}
	if (ctx->ambient_count != 1)
	{
		print_parse_error(ctx->filename, ctx->current_line,
			"Expected exactly 1 ambient light");
		return (false);
	}
	if (ctx->light_count < 1)
	{
		print_parse_error(ctx->filename, ctx->current_line,
			"No point lights defined");
		return (false);
	}
	return (true);
}

bool	process_line(t_parse_context *context, t_scene *scene, const char *line)
{
	int		i;
	char	*identifier;

	if (ft_isempty_or_comment(line))
		return (true);
	i = 0;
	identifier = extract_identifier(line);
	while (g_element_parsers[i].identifier)
	{
		if (ft_strcmp(identifier, g_element_parsers[i].identifier) == 0)
		{
			if (!g_element_parsers[i].parser(context, scene, line))
			{
				context->error_occurred = true;
				free(identifier);
				return (false);
			}
			free(identifier);
			return (true);
		}
		i++;
	}
	context->error_occurred = true;
	free(identifier);
	return (false);
}

bool	parse_scene(const char *filename, t_scene *scene)
{
	char			*file_content;
	t_parse_context	parse_context;
	int				fd;

	parse_context = ft_init_parse_context(filename);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (false);
	file_content = get_next_line(fd);
	while (file_content)
	{
		parse_context.current_line++;
		if (!process_line(&parse_context, scene, file_content))
		{
			free(file_content);
			break ;
		}
		free(file_content);
		file_content = get_next_line(fd);
	}
	close(fd);
	if (parse_context.error_occurred)
		return (false);
	return (validate_scene_counts(&parse_context));
}
