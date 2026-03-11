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

#include "parser_bonus.h"

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

static bool	dispatch_parser(t_parse_context *ctx, t_scene *scene,
				const char *line, char *id)
{
	int	i;

	i = 0;
	while (g_element_parsers[i].identifier)
	{
		if (ft_strcmp(id, g_element_parsers[i].identifier) == 0)
		{
			if (!g_element_parsers[i].parser(ctx, scene, line))
				ctx->error_occurred = true;
			return (!ctx->error_occurred);
		}
		i++;
	}
	print_parse_error(ctx->filename, ctx->current_line, id);
	ctx->error_occurred = true;
	return (false);
}

static void	read_scene_file(t_parse_context *ctx, t_scene *scene, int fd)
{
	char	*file_content;
	char	*trimmed_line;

	file_content = get_next_line(fd);
	while (file_content)
	{
		ctx->current_line++;
		trimmed_line = ft_strtrim(file_content, "\n\r");
		if (!process_line(ctx, scene, trimmed_line))
		{
			free(trimmed_line);
			free(file_content);
			return ;
		}
		free(trimmed_line);
		free(file_content);
		file_content = get_next_line(fd);
	}
}

bool	process_line(t_parse_context *context, t_scene *scene, const char *line)
{
	char	*identifier;
	bool	result;

	if (ft_isempty_or_comment(line))
		return (true);
	identifier = extract_identifier(line);
	result = dispatch_parser(context, scene, line, identifier);
	free(identifier);
	return (result);
}

bool	parse_scene(const char *filename, t_scene *scene)
{
	t_parse_context	ctx;
	int				fd;

	ctx = (t_parse_context){0};
	ctx.filename = filename;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (false);
	read_scene_file(&ctx, scene, fd);
	close(fd);
	if (ctx.error_occurred)
		return (false);
	return (validate_scene_counts(&ctx));
}
