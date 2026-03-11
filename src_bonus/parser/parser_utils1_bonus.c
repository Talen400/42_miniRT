/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-04 16:48:48 by rgregori          #+#    #+#             */
/*   Updated: 2026-03-04 16:48:48 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_bonus.h"

bool	valid_obj_token_count(size_t count, int base)
{
	int	extra;

	extra = (int)count - base;
	return (extra == 0 || extra == B_PHONG_NARGS || extra == B_REFL_NARGS
		|| extra == B_CHECKER_NARGS || extra == B_BUMP_NARGS);
}
