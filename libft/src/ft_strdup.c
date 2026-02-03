/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 14:27:44 by rgregori          #+#    #+#             */
/*   Updated: 2025/07/21 12:10:36 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*cop;
	int		i;

	cop = malloc(sizeof(*src) * (ft_strlen(src) + 1));
	if (!cop)
		return (NULL);
	i = 0;
	while (src[i])
	{
		cop[i] = src[i];
		i++;
	}
	cop[i] = '\0';
	return (cop);
}
