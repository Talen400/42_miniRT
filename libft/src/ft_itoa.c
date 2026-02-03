/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 12:52:33 by rgregori          #+#    #+#             */
/*   Updated: 2025/07/16 16:04:26 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static int	ft_calc_size(int n)
{
	int		is_negative;
	int		count;
	long	nb;

	nb = (long)n;
	is_negative = (nb < 0);
	count = 0;
	if (is_negative)
		nb = -nb;
	if (n == 0)
		count++;
	while (nb > 0)
	{
		nb /= 10;
		count++;
	}
	return (count + is_negative);
}

static char	*ft_fill_str(char *s, int len, int n)
{
	int		i;
	long	nb;

	if (n == 0)
	{
		s[0] = '0';
		s[1] = '\0';
		return (s);
	}
	nb = (long)n;
	if (nb < 0)
	{
		s[0] = '-';
		nb = -nb;
	}
	s[len] = '\0';
	i = len - 1;
	while (nb > 0)
	{
		s[i--] = ((long)nb % 10 + '0');
		nb /= 10;
	}
	return (s);
}

char	*ft_itoa(int n)
{
	char	*str_nbr;
	int		total_len;

	total_len = ft_calc_size(n);
	str_nbr = malloc((total_len + 1) * sizeof(char));
	if (!str_nbr)
		return (NULL);
	return (ft_fill_str(str_nbr, total_len, n));
}
