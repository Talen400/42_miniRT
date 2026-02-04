/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:49:32 by rgregori          #+#    #+#             */
/*   Updated: 2025/07/16 16:06:55 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_charinstr(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t		start;
	size_t		end;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_charinstr(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_charinstr(s1[end - 1], set))
		end--;
	return (ft_substr(s1, start, end - start));
}
