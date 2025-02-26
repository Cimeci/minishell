/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:12:58 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/26 10:16:22 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst	*ft_unset_extension(char *unset_path, t_lst *env)
{
	t_lst	*tmp;
	t_lst	*prev;
	char	*var;

	tmp = env;
	prev = NULL;
	while (tmp)
	{
		var = ft_get_var(tmp->str);
		if (ft_strcmp(var, unset_path))
		{
			if (prev == NULL)
				env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->str);
			free(tmp);
			free(var);
			tmp = NULL;
			return (env);
		}
		free(var);
		prev = tmp;
		tmp = tmp->next;
	}
	return (NULL);
}

int	ft_unset(t_data *data, t_cmd *cur)
{
	int	i;

	i = 1;
	while (cur->args[i])
	{
		data->env = ft_unset_extension(cur->args[i], data->env);
		i++;
	}
	if (data->env && data->env->str && data->env->str[0] == '\0')
	{
		data->env = NULL;
	}
	return (1);
}
