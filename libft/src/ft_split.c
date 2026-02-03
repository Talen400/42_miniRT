/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:29:02 by rgregori          #+#    #+#             */
/*   Updated: 2025/07/21 11:42:19 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *str, char charset)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if ((str[i] != charset)
			&& (i == 0 || (str[i - 1] == charset)))
		{
			count++;
		}
		i++;
	}
	return (count);
}

static void	ft_free_result(char **result, int z)
{
	int	i;

	i = 0;
	while (i < z)
		free(result[i++]);
	free(result);
}

static int	copy_word_to_result(char **result, char const *src,
	char set, int *z)
{
	int		len;
	char	*temp;

	len = 0;
	while (src[len] && src[len] != set)
		len++;
	temp = ft_substr(src, 0, len);
	if (!temp)
		return (0);
	result[(*z)++] = temp;
	return (len);
}

static int	ft_fill_split(char **result, char const *src, char set)
{
	int	i;
	int	z;
	int	offset;

	i = 0;
	z = 0;
	while (src[i])
	{
		if (src[i] != set && (i == 0 || src[i - 1] == set))
		{
			offset = copy_word_to_result(result, src + i, set, &z);
			if (!offset)
			{
				ft_free_result(result, z);
				return (0);
			}
			i += offset;
		}
		else
			i++;
	}
	result[z] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**result;

	if (!s)
		return (NULL);
	result = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	if (!ft_fill_split(result, s, c))
	{
		return (NULL);
	}
	return (result);
}
