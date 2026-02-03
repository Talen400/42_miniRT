/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:14:35 by rgregori          #+#    #+#             */
/*   Updated: 2025/07/16 11:47:29 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

char	*ft_strrchr(const char *s, int c)
{
	char	*hold;

	hold = NULL;
	while (*s)
	{
		if (*s == (char)(unsigned char)c)
			hold = (char *)s;
		s++;
	}
	if ((char)(unsigned char)c == '\0')
		return ((char *)s);
	return (hold);
}
