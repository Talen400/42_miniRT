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
