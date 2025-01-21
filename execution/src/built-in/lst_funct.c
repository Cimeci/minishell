/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_funct.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:22:33 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/21 16:56:26 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_lst	*ft_dup_lst(t_lst *env)
{
	t_lst	*export_env;
	t_lst	*cur;
	t_lst	*s_export_env;

	cur = env;
	export_env = NULL;
	while (env)
	{
		s_export_env = malloc(sizeof(t_lst));
		s_export_env->str = ft_strdup(env->str);
		s_export_env->next = NULL;
		ft_lstadd_back2(&export_env, s_export_env);
		env = env->next;
	}
	return (export_env);
}

char	**ft_convert_lst_to_tab(t_lst *env)
{
	int		i;
	char	**table;
	t_lst	*tmp;

	i = 0;
	table = malloc(sizeof(char *) * (ft_lstsize2(env) + 1));
	if (!table)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		table[i] = ft_strdup(tmp->str);
		i++;
		tmp = tmp->next;
	}
	table[i] = NULL;
	return (table);
}

t_lst	*ft_init_env(char **env)
{
	t_lst	*s_env;
	t_lst	*list;
	int		i;

	i = 0;
	list = NULL;
	while (env[i])
	{
		s_env = malloc(sizeof(t_lst));
		s_env->str = ft_strdup(env[i++]);
		s_env->next = NULL;
		ft_lstadd_back2(&list, s_env);
	}
	return (list);
}

char	*my_getenv_lst(const char *name, t_lst *env)
{
	int		j;
	char	*tmp;
	char	*tmp_dup;
	t_lst	*cur;

	cur = env;
	while (cur)
	{
		j = 0;
		while (cur->str[j] != '=' && cur->str[j])
			j++;
		tmp_dup = ft_strdup(cur->str);
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
