/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:15:03 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/18 15:14:51 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	setup_execution(t_data *data)
{
	data->original_stdin = dup(STDIN_FILENO);
	data->original_stdout = dup(STDOUT_FILENO);
	signal(SIGINT, parent_signal_handler_exec);
	signal(SIGQUIT, parent_signal_handler_exec);
	data->nb_cmd = ft_lstsize_generic((void *)data->cmd, sizeof(t_cmd)
			- sizeof(t_cmd *));
	int info = handle_unique_builtin(data, data->cmd);
	if (info)
		return (1);
	return (0);
}

void	execute_pipeline(t_data *data)
{
	int		i;
	t_cmd	*cur;
	pid_t	p;

	i = 0;
	cur = data->cmd;
	while (cur && i < data->nb_cmd)
	{
		if (!files(data, cur) && cur->empty_var_cmd == false)
		{
			if (pipe(data->fd) == -1)
				printf("pipe failed\n");
			if (cur->here == 1)
				signal(SIGINT, parent_signal_handler_here);
			p = fork();
			if (p < 0)
				printf("fork failed\n");
			else if (p == 0)
				child(data, cur, i);
			else
				parent(data);
		}
		i++;
		cur = cur->next;
	}
}

void	exec(t_data *data)
{
	if (!data->cmd)
		return ;
	if (setup_execution(data))
	{
		cleanup_execution(data);
		return ;
	}
	execute_pipeline(data);
	handle_parent_process(data);
	cleanup_execution(data);
}
