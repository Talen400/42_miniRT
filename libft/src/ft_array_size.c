/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_size.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-05 17:56:31 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-05 17:56:31 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_array_size(void **array)
{
	size_t	count;

	if (!array)
		return (0);
	count = 0;
	while (array[count])
		count++;
	return (count);
}
