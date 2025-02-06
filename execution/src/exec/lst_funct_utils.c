/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_funct_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:23:53 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/06 14:14:00 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_lstnew_generic(size_t data_size)
{
	void	*new_node;

	new_node = malloc(data_size);
	if (!new_node)
		return (NULL);
	memset(new_node, 0, data_size);
	return (new_node);
}

void	ft_lstadd_back_generic(void **lst, void *new_node, size_t next_offset)
{
	void	*temp;

	if (!lst || !new_node)
		return ;
	temp = *lst;
	if (*lst == NULL)
	{
		*lst = new_node;
		*(void **)((char *)new_node + next_offset) = NULL;
		return ;
	}
	while (*(void **)((char *)temp + next_offset) != NULL)
		temp = *(void **)((char *)temp + next_offset);
	if (temp)
		*(void **)((char *)temp + next_offset) = new_node;
}

int	ft_lstsize_generic(void *lst, size_t offset)
{
	int		i;
	void	*cur;
	void	**next;

	i = 0;
	cur = lst;
	while (cur)
	{
		next = (void **)((char *)cur + offset);
		cur = *next;
		i++;
	}
	return (i);
}

void	*ft_lstlast_generic(void *lst, size_t offset)
{
	void	**last;
	void	*cur;

	if (!lst)
		return (NULL);
	cur = lst;
	while (*(void **)((char *)cur + offset) != NULL)
	{
		last = (void **)((char *)cur + offset);
		cur = *last;
	}
	return (cur);
}
