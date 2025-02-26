/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_funct.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:22:33 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/26 11:42:40 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_convert_lst_to_tab(t_lst *env)
{
	int		i;
	char	**table;
	t_lst	*tmp;

	i = 0;
	table = malloc(sizeof(char *) * (ft_lstsize_generic((void *)env,
					sizeof(char *)) + 1));
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
