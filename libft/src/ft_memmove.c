/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mem2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:29:25 by rgregori          #+#    #+#             */
/*   Updated: 2025/07/11 10:54:32 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

static void	*ft_move(unsigned char *dest, const unsigned char *src, size_t n)
{
	size_t	i;

	if (dest < src)
	{
		i = 0;
		while (i < n)
		{
			dest[i] = src[i];
			i++;
		}
	}
	else if (dest > src)
	{
		i = n;
		while (i > 0)
		{
			dest[i - 1] = src[i - 1];
			i--;
		}
	}
	return (dest);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (!dest || !src)
		return (NULL);
	return (ft_move(d, s, n));
}
