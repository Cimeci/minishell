/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:52:47 by ncharbog          #+#    #+#             */
/*   Updated: 2024/10/17 16:30:51 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*destlist;
	t_list	*new;
	void	*temp;

	new = NULL;
	destlist = NULL;
	temp = NULL;
	if (!lst || !f || !del)
		return (0);
	while (lst)
	{
		temp = f(lst->content);
		if (temp)
			new = ft_lstnew(temp);
		if (new == NULL || temp == NULL)
		{
			if (temp)
				del(temp);
			ft_lstclear(&destlist, del);
			return (0);
		}
		ft_lstadd_back(&destlist, new);
		lst = lst->next;
	}
	return (destlist);
}
