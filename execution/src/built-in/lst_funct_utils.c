/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_funct_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:23:53 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/21 16:48:52 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_lstclear2(t_lst *env)
{
	t_lst	*temp;

	temp = env;
	while (env)
	{
		temp = env->next;
		if (env->str)
			free(env->str);
		free(env);
		env = temp;
	}
}

int	ft_lstsize2(t_lst *lst)
{
	int	i;

	i = 0;
	if (!lst)
		return (0);
	if (lst->next == NULL)
		return (1);
	while (lst->next != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i + 1);
}

t_lst	*ft_lstnew2(void *path)
{
	t_lst	*new;

	new = malloc(sizeof(t_lst));
	if (new == NULL)
		return (0);
	new->str = path;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back2(t_lst **lst, t_lst *new)
{
	t_lst	*temp;

	if (!lst || !new)
		return ;
	temp = *lst;
	if (!temp)
	{
		*lst = new;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}
