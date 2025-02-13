/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_check_print.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:19:07 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/13 15:20:11 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_check_isalnum(t_data *data, char *var, int i)
{
	if (!ft_isalnum(var[i]))
	{
		if (var[i] != '_')
		{
			errors_exec(var, "export", IDENTIFIER);
			data->gexit_code = 1;
			return (1);
		}
	}
	return (0);
}

int	ft_check_env_var(t_data *data, char *var)
{
	int	i;
	int	len;

	if (!var)
		return (1);
	len = ft_strlen(var) - 1;
	i = 1;
	if (!ft_isalpha(var[0]))
	{
		if (var[0] != '_')
		{
			errors_exec(var, "export", IDENTIFIER);
			data->gexit_code = 1;
			return (1);
		}
	}
	while (i <= len)
	{
		if (ft_check_isalnum(data, var, i))
			return (1);
		i++;
	}
	return (0);
}

void	ft_print_export_env(t_lst *export_env)
{
	t_lst	*tmp;

	tmp = export_env;
	while (tmp)
	{
		printf("%s\n", tmp->str);
		tmp = tmp->next;
	}
}

int	print_export(t_data *data, t_cmd *cur)
{
	if (ft_strlen_tab(cur->args) == 1)
	{
		ft_print_export_env(data->export_env);
		return (0);
	}
	return (1);
}
