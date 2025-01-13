/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_funct_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:23:53 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 11:39:20 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstclear2(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (env)
	{
		temp = env->next;
		if (env->path)
			free(env->path);
		free(env);
		env = temp;
	}
}

int	ft_lstsize2(t_env *lst)
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

t_env	*ft_lstnew2(void *path)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (new == NULL)
		return (0);
	new->path = path;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back2(t_env **lst, t_env *new)
{
	t_env	*temp;

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
