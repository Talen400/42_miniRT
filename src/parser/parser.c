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

#include "miniRt.h"

bool	parse_scene(const char *filename, t_scene *scene)
{
	char *file_content;
	int	 fd;

	(void)scene; 
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (false);
	file_content = get_next_line(fd);
	while (file_content)
	{
		
		// Aqui você processaria cada linha do arquivo
		// Por exemplo, identificar o tipo de elemento (C, L, sp, pl, etc)
		// e chamar a função de parsing correspondente
		free(file_content);
		file_content = get_next_line(fd);
	}
	return (true);
}
