/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:12:58 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/13 14:51:19 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_unset_extension(char *unset_path, t_lst *env)
{
	t_lst	*tmp;
	t_lst	*prev;
	char	*var;

	tmp = env;
	prev = NULL;
	while (tmp)
	{
		var = ft_get_var(tmp->str);
		if (!ft_strncmp(var, unset_path, ft_strlen(var)))
		{
			if (prev == NULL)
				env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->str);
			free(tmp);
			free(var);
			tmp = NULL;
			break ;
		}
		free(var);
		prev = tmp;
		tmp = tmp->next;
	}
}

int	ft_unset(t_data *data, t_cmd *cur)
{
	int	i;

	i = 1;
	while (cur->args[i])
	{
		ft_unset_extension(cur->args[i], data->env);
		i++;
	}
	return (1);
}
