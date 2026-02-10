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

static bool	validate_color_values(int *values, char **components)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		values[i] = ft_atoi(components[i]);
		if (values[i] < 0 || values[i] > 255)
			return (false);
		i++;
	}
	return (true);
}

bool	parse_vector(const char *str, t_vec3 *vec)
{
	char	**components;

	components = ft_split(str, ',');
	if (!components)
		return (false);
	if (ft_array_size((void **)components) != 3)
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
	int		values[3];

	components = ft_split(str, ',');
	if (!components || ft_array_size((void **)components) != 3)
	{
		if (components)
			ft_free_split(components);
		return (false);
	}
	if (!validate_color_values(values, components))
	{
		ft_free_split(components);
		return (false);
	}
	color->r = (double)values[0] / 255.0;
	color->g = (double)values[1] / 255.0;
	color->b = (double)values[2] / 255.0;
	ft_free_split(components);
	return (true);
}

