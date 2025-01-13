/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_funct.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:44:07 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 11:48:02 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_env	*ft_init_env(char **env)
{
	t_env	*s_env;
	t_env	*list;
	int		i;

	i = 0;
	list = NULL;
	while (env[i])
	{
		s_env = malloc(sizeof(t_env));
		s_env->path = ft_strdup(env[i++]);
		s_env->next = NULL;
		ft_lstadd_back2(&list, s_env);
	}
	return (list);
}

char	**ft_convert_lst_to_tab(t_env *env)
{
	int		i;
	char	**table;
	t_env	*tmp;

	i = 0;
	table = malloc(sizeof(char *) * (ft_lstsize2(env) + 1));
	tmp = env;
	while (tmp)
	{
		table[i] = ft_strdup(tmp->path);
		i++;
		tmp = tmp->next;
	}
	table[i] = NULL;
	return (table);
}

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
