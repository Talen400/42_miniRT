/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:30:32 by rgregori          #+#    #+#             */
/*   Updated: 2025/08/04 15:10:31 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

int	ft_putnbr_fd(int n, int fd)
{
	int		size;
	long	nb;

	size = 0;
	nb = n;
	if (nb < 0)
	{
		size += ft_putchar_fd('-', fd);
		nb = -nb;
	}
	if (nb > 9)
		size += ft_putnbr_fd(nb / 10, fd);
	size += ft_putchar_fd(nb % 10 + '0', fd);
	return (size);
}
