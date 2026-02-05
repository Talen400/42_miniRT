/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 15:07:34 by rgregori          #+#    #+#             */
/*   Updated: 2026-01-29 15:07:34 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool ft_isempty_or_comment(const char *line)
{
	while (*line && ft_isspace(*line))
		line++;
	return (!line[0] || line[0] == '\n' || line[0] == '#');
}

char *extract_identifier(const char *line)
{
	char	*id;
	int		len;

	len = 0;
	while (line[len] && !ft_isspace(line[len]))
		len++;
	id = (char *)malloc(len + 1);
	if (!id)
		return (NULL);
	ft_strlcpy(id, line, len + 1);
	return (id);
}

bool	parse_vector(const char *str, t_vec3 *vec)
{
	char	**components;

	components = ft_split(str, ',');
	if (!components)
		return (false);
	if (!components[0] || !components[1] || !components[2]
		|| components[3] != NULL)
	{
		ft_free_split(components);
		return (false);
	}
	vec->x = ft_atod(components[0]);
	vec->y = ft_atod(components[1]);
	vec->z = ft_atod(components[2]);
	ft_free_split(components);
	return (true);
}

bool	parse_color(const char *str, t_color *color)
{
	char	**components;

	components = ft_split(str, ',');
	if (!components)
		return (false);
	if (!components[0] || !components[1] || !components[2]
		|| components[3] != NULL)
	{
		ft_free_split(components);
		return (false);
	}
	color->r = ft_atoi(components[0]);
	color->g = ft_atoi(components[1]);
	color->b = ft_atoi(components[2]);
	if (color->r < 0 || color->r > 255 || color->g < 0 || color->g > 255 || color->b < 0 || color->b > 255)
	{
		ft_free_split(components);
		return (false);
	}
	ft_free_split(components);
	return (true);
}

