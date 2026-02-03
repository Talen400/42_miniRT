/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 13:41:19 by rgregori          #+#    #+#             */
/*   Updated: 2025/08/29 13:50:42 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


static int	ft_atod_sign(const char *str, int *i)
{
	while ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == ' ')
		(*i)++;
	if (str[*i] == '-')
		return ((*i)++, -1);
	if (str[*i] == '+')
		(*i)++;
	return (1);
}

static double	ft_atod_int(const char *str, int *i)
{
	double	n;

	n = 0.0;
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		n = n * 10.0 + (str[*i] - '0');
		(*i)++;
	}
	return (n);
}

static double	ft_atod_frac(const char *str, int *i)
{
	double	n;
	double	div;

	n = 0.0;
	div = 10.0;
	if (str[*i] == '.')
	{
		(*i)++;
		while (str[*i] >= '0' && str[*i] <= '9')
		{
			n += (str[*i] - '0') / div;
			div *= 10.0;
			(*i)++;
		}
	}
	return (n);
}

double	ft_atod(const char *str)
{
	int		i;
	int		sign;
	double	result;

	i = 0;
	sign = ft_atod_sign(str, &i);
	result = ft_atod_int(str, &i);
	result += ft_atod_frac(str, &i);
	return (result * sign);
}
