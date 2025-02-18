/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:02:20 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/18 10:40:54 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_commande_execution(t_data *data, t_cmd *cur)
{
	if (!exec_built_in(data, cur))
	{
		if (execve(cur->cmd, cur->args, ft_convert_lst_to_tab(data->env)) == -1)
		{
			if (opendir(cur->cmd) != NULL)
			{
				errors(data, cur->args[0], DIRECTORY);
				exit(126);
			}
			if (access(cur->cmd, X_OK) != 0)
			{
				if (errno == EACCES)
				{
					errors(data, cur->args[0], PERM);
					exit(126);
				}
				errors(data, cur->args[0], CMD_NOT_FOUND);
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
