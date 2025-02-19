/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:02:20 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/19 16:07:57 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_commande_execution(t_data *data, t_cmd *cur)
{
	if (!exec_built_in(data, cur))
	{
		if ((!ft_strncmp(cur->cmd, ".", 1) && ft_strlen(cur->cmd) == 1)
			|| (!ft_strncmp(cur->cmd, "..", 2) && ft_strlen(cur->cmd) == 2))
		{
			errors(data, cur->args[0], CMD_NOT_FOUND);
			if (!ft_strncmp(cur->cmd, ".", 1) && ft_strlen(cur->cmd) == 1)
				exit(2);
			exit(127);
		}
		if (opendir(cur->cmd) != NULL)
		{
			errors(data, cur->args[0], DIRECTORY);
			exit(126);
		}
		if (my_getenv_lst("PATH", data->env) && !ft_strnstr(cur->cmd, "/", ft_strlen(cur->cmd)))
		{
			errors(data, cur->args[0], CMD_NOT_FOUND);
			exit(127);
		}
		if (access(cur->cmd, F_OK) == 0)
		{
			if (access(cur->cmd, X_OK) != 0)
			{
				errors(data, cur->args[0], PERM);
				exit(126);
			}
		}
		if (access(cur->cmd, X_OK))
		{
			errors(data, cur->args[0], CMD_NOT_FOUND);
			exit(127);
		}
		execve(cur->cmd, cur->args, data->env_cp);
	}
}

int	handle_here_doc(t_data *data, t_cmd *cur)
{
	if (cur->here == 1)
	{
		cur->file = randomizer();
		if (!cur->file)
		{
			data->gexit_code = 1;
			return (1);
		}
	}
	return (0);
}
