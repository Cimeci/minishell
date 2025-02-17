/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:02:20 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/17 13:45:24 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_exec_cmd(t_data *data, t_cmd *cur)
{
	char	*tmp;

	tmp = NULL;
	if (!ft_strnstr(cur->cmd, "/", ft_strlen(cur->cmd)))
	{
		if (!my_getenv_lst("PATH", data->env))
		{
			tmp = ft_strdup(cur->cmd);
			cur->cmd = ft_strjoin("./", tmp);
			free(tmp);
		}
		else
		{
			errors(data, cur->cmd, CMD_NOT_FOUND);
			exit(127);
		}
	}
}

void	handle_commande_execution(t_data *data, t_cmd *cur)
{
	if (!exec_built_in(data, cur))
	{
		check_exec_cmd(data, cur);
		if (execve(cur->cmd, cur->args, ft_convert_lst_to_tab(data->env)) == -1)
		{
			if (opendir(cur->cmd) != NULL)
			{
				errors(data, cur->cmd, DIRECTORY);
				exit(126);
			}
			if (access(cur->cmd, X_OK) != 0)
			{
				if (errno == EACCES)
				{
					errors(data, cur->cmd, PERM);
					exit(126);
				}
				errors(data, cur->cmd, CMD_NOT_FOUND);
				exit(127);
			}
		}
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
