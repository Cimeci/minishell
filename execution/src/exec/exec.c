/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:15:03 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/22 11:22:30 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_built_in(t_data *data, t_cmd *cur)
{
	if (!ft_strncmp(cur->cmd, "cd", ft_strlen(cur->cmd))
		&& ft_strlen(cur->cmd) == 2)
		ft_cd(cur->args);
	else if (!ft_strncmp(cur->args[0], "pwd", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 3)
		ft_pwd();
	else if (!ft_strncmp(cur->args[0], "echo", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 4)
		ft_echo(ft_strlen_tab(cur->args), cur->args);
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
	// execution_cmd(data);
	return (1);
}

void	exec(t_data *data)
{
	t_cmd	*cur;

	cur = data->cmd;
	while (cur)
	{
		if (!is_built_in(data, cur))
			return ;
		cur = cur->next;
	}
}
