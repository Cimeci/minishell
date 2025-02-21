/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:40:57 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/21 15:09:31 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_modif_env_var(t_lst *cur, char *var, char *arg)
{
	char	*path;

	while (cur)
	{
		path = ft_get_var(cur->str);
		if (path)
		{
			if (!ft_strncmp(var, path, ft_strlen(var)))
			{
				free(cur->str);
				free(path);
				cur->str = ft_strdup(arg);
				break ;
			}
		}
		free(path);
		path = NULL;
		cur = cur->next;
	}
}

int	ft_export_append(t_data *data, char *arg)
{
	char	*var;
	char	*path;
	char	*current_value;
	char	*new_value;
	char	*full_var;

	full_var = NULL;
	current_value = NULL;
	var = ft_get_pvar(arg);
	path = my_getenv_lst(var, data->env);
	new_value = ft_get_value(arg);
	if (!path)
		ft_export_assign(data, var, new_value);
	else
	{
		current_value = ft_get_var_and_value(path, data->env);
		full_var = ft_strjoin(current_value, new_value);
		ft_modif_env_var(data->env, var, full_var);
		free(full_var);
	}
	free(var);
	free(path);
	free(new_value);
	return (1);
}

int	ft_export_assign(t_data *data, char *var, char *value)
{
	char	*path;
	char	*full_arg;
	char	*var_egal;

	path = my_getenv_lst(var, data->env);
	var_egal = ft_strjoin(var, "=");
	full_arg = ft_strjoin(var_egal, value);
	free(var_egal);
	if (!path)
	{
		if (!ft_add_to_list(&data->env, full_arg))
			return (0);
	}
	else
	{
		ft_modif_env_var(data->env, var, full_arg);
		free(path);
	}
	free(full_arg);
	return (1);
}
