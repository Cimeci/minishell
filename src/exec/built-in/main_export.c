/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/21 15:09:55 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_regular_export(t_data *data, char *arg, char *var)
{
	char	*value;

	if (ft_check_env_var(data, var))
		return (1);
	value = ft_get_value(arg);
	ft_export_assign(data, var, value);
	free(value);
	return (0);
}

static int	handle_plus_export(t_data *data, char *arg, char *pvar)
{
	if (ft_check_env_var(data, pvar))
		return (1);
	ft_export_append(data, arg);
	return (0);
}

static int	process_export_arg(t_data *data, char *arg)
{
	char	*var;
	char	*pvar;
	int		info;

	info = 0;
	var = ft_get_var(arg);
	pvar = ft_get_pvar(arg);
	if (pvar)
		info = handle_plus_export(data, arg, pvar);
	else if (var && !info)
		info = handle_regular_export(data, arg, var);
	else if (!info)
		ft_check_env_var(data, arg);
	free(var);
	free(pvar);
	return (info);
}

int	ft_export(t_data *data, t_cmd *cur)
{
	int	i;

	data->gexit_code = 0;
	if (ft_strlen_tab(cur->args) == 1)
	{
		print_errors(cur->cmd, ": no argument", NULL);
		data->gexit_code = 2;
		return (1);
	}
	i = 1;
	while (cur->args[i])
	{
		process_export_arg(data, cur->args[i]);
		i++;
	}
	return (0);
}
