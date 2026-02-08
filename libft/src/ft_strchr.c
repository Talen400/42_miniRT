/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 12:36:40 by rgregori          #+#    #+#             */
/*   Updated: 2025/07/16 11:31:49 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)(unsigned char)c)
			return ((char *)s);
		s++;
	}
	if ((char)(unsigned char)c == '\0')
		return ((char *)s);
	return (NULL);
}
