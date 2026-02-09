/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-05 16:37:43 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-05 16:37:43 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"


void	print_parse_error(const char *filename, int line_num, 
			const char *message)
{
	char *line_str;

	write(2, "Error\n", 6);
	write(2, "File '", 7);
	write(2, filename, ft_strlen(filename));
	write(2, "' line ", 7);
	line_str = ft_itoa(line_num);
	write(2, line_str, ft_strlen(line_str));
	free(line_str);
	write(2, ": ", 2);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
}

void	ft_error_and_free(t_parse_context *ctx, char **tokens, const char *message)
{
	print_parse_error(ctx->filename, ctx->current_line, message);
	ft_free_split(tokens);
}
