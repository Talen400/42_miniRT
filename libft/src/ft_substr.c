/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:35:25 by rgregori          #+#    #+#             */
/*   Updated: 2025/07/16 10:54:41 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*sub;
	size_t		s_len;
	size_t		alloc_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start > s_len)
		return (ft_strdup(""));
	alloc_len = s_len - start;
	if (alloc_len > len)
		alloc_len = len;
	sub = malloc(alloc_len + 1);
	if (!sub)
		return (NULL);
	ft_strlcpy(sub, s + start, alloc_len + 1);
	return (sub);
}
