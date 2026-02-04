/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:16:41 by rgregori          #+#    #+#             */
/*   Updated: 2025/07/17 13:55:20 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*next_item;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		next_item = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = next_item;
	}
	*lst = NULL;
}
