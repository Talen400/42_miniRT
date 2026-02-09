/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:31:38 by rgregori          #+#    #+#             */
/*   Updated: 2025/07/17 13:58:27 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *),
void (*del)(void *))
{
	t_list	*new_item;
	t_list	*new_list;

	new_list = NULL;
	if (!lst || !f || !del)
		return (NULL);
	while (lst)
	{
		new_item = ft_lstnew(f(lst->content));
		if (!new_item)
		{
			ft_lstclear(&new_item, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_item);
		lst = lst->next;
	}
	return (new_list);
}
