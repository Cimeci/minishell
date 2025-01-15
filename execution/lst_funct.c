/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_funct.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:22:33 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/15 10:36:12 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_convert_lst_to_tab(t_env *env)
{
	int		i;
	char	**table;
	t_env	*tmp;

	i = 0;
	table = malloc(sizeof(char *) * (ft_lstsize2(env) + 1));
	if (!table)
		return (NULL);
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

char	*my_getenv_lst(const char *name, t_env *env)
{
	int		j;
	char	*tmp;
	char	*tmp_dup;
	t_env	*cur;

	cur = env;
	while (cur)
	{
		j = 0;
		while (cur->path[j] != '=' && cur->path[j])
			j++;
		tmp_dup = ft_strdup(cur->path);
		tmp = ft_substr(tmp_dup, 0, j);
		if (!ft_strncmp(tmp, name, ft_strlen(name)))
		{
			free(tmp_dup);
			return (tmp);
		}
		free(tmp_dup);
		free(tmp);
		cur = cur->next;
	}
	return (NULL);
}
