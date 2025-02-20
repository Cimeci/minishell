/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:00:08 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/20 15:54:19 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_built_in(t_data *data, t_cmd *cur)
{

	if (!ft_strncmp(cur->cmd, "exit", ft_strlen(cur->cmd))
		&& ft_strlen(cur->cmd) == 4)
		ft_exit(data, cur);
	if (!ft_strncmp(cur->cmd, "cd", ft_strlen(cur->cmd))
		&& ft_strlen(cur->cmd) == 2)
		ft_cd(data, cur);
	else if (!ft_strncmp(cur->args[0], "pwd", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 3)
		ft_pwd(data);
	else if (!ft_strncmp(cur->args[0], "echo", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 4)
		ft_echo(cur->args);
	else if (!ft_strncmp(cur->args[0], "env", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 3)
		ft_env(data, cur);
	else if (!ft_strncmp(cur->args[0], "export", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 6)
		ft_export(data, cur);
	else if (!ft_strncmp(cur->args[0], "unset", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 5)
		ft_unset(data, cur);
	else
		return (0);
	return (1);
}

int	handle_unique_builtin(t_data *data, t_cmd *cur)
{
	if (data->nb_cmd == 1)
	{
		if (cur->cmd && !ft_strncmp(cur->cmd, "exit", ft_strlen(cur->cmd))
			&& ft_strlen(cur->cmd) == 4)
		{
			ft_exit(data, cur);
			return (1);
		}
		else if (cur->cmd && !ft_strncmp(cur->cmd, "cd", ft_strlen(cur->cmd))
			&& ft_strlen(cur->cmd) == 2)
		{
			ft_cd(data, cur);
			return (1);
		}
		else if (cur->cmd && !ft_strncmp(cur->args[0], "export",
				ft_strlen(cur->args[0])) && ft_strlen(cur->args[0]) == 6)
		{
			ft_export(data, cur);
			return (1);
		}
		else if (cur->cmd && !ft_strncmp(cur->args[0], "unset",
				ft_strlen(cur->args[0])) && ft_strlen(cur->args[0]) == 5)
		{
			ft_unset(data, cur);
			return (1);
		}
	}
	return (0);
}
