/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:12:58 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/21 16:34:54 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_unset_extension(char *unset_path, t_env *env)
{
	t_env	*tmp;
	t_env	*prev;
	tmp = env;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strncmp(tmp->path, unset_path, ft_strlen(tmp->path)))
		{
			if (prev == NULL)
				env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->path);
			free(tmp);
			tmp = NULL;
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}
int	ft_unset(t_data *data, t_cmd *cur)
{
	char	*unset_path;
	int		i;
	i = 1;
	while (cur->args[i])
	{
		unset_path = get_var(cur->args[i], data->env);
		if (!unset_path)
			return (0);
		ft_unset_extension(unset_path, data->env);
		i++;
	}
	return (1);
}