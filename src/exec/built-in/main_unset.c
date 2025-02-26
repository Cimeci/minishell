/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:12:58 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/26 10:54:20 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst	*ft_unset_extension(char *unset_path, t_lst *env, char *var)
{
	t_lst	*prev;
	t_lst	*head;

	head = env;
	prev = NULL;
	while (env)
	{
		var = ft_get_var(env->str);
		if (ft_strcmp(var, unset_path))
		{
			if (prev == NULL)
				head = env->next;
			else
				prev->next = env->next;
			free(env->str);
			free(env);
			free(var);
			env = NULL;
			return (head);
		}
		free(var);
		prev = env;
		env = env->next;
	}
	return (head);
}

int	ft_unset(t_data *data, t_cmd *cur)
{
	int		i;
	char	*var;

	var = NULL;
	i = 1;
	while (cur->args[i])
	{	
		data->env = ft_unset_extension(cur->args[i], data->env, var);
		i++;
	}
	return (1);
}
