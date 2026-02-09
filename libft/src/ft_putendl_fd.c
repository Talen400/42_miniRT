/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:28:12 by rgregori          #+#    #+#             */
/*   Updated: 2025/08/04 16:15:26 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putendl_fd(const char *s, int fd)
{
	int	size;

	size = 0;
	size += ft_putstr_fd(s, fd);
	size += ft_putchar_fd('\n', fd);
	return (size);
}
