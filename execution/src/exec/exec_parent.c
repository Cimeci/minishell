/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parent.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:08:15 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/13 17:17:42 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parent(t_data *data)
{
	close(data->fd[1]);
	dup2(data->fd[0], STDIN_FILENO);
	close(data->fd[0]);
}

void	handle_parent_process(t_data *data)
{
	int	status;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status) && data->gexit_code == 0)
			data->gexit_code = WEXITSTATUS(status);
	}
	signal(SIGINT, parent_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	cleanup_execution(t_data *data)
{
	close(data->fd[0]);
	close(data->fd[1]);
	dup2(data->original_stdin, STDIN_FILENO);
	close(data->original_stdin);
	dup2(data->original_stdout, STDOUT_FILENO);
	close(data->original_stdout);
}
