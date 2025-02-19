/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parent.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:08:15 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/19 16:58:15 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parent(t_data *data)
{
	close(data->fd[1]);
	dup2(data->fd[0], STDIN_FILENO);
	close(data->fd[0]);
}

void	handle_parent_process(t_data *data, t_cmd *last)
{
	int	status;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			data->gexit_code = WEXITSTATUS(status);
	}
	if (last->fd_infile < 0 || last->fd_outfile < 0)
		data->gexit_code = 1;
	signal(SIGINT, parent_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	cleanup_execution(t_data *data)
{
	if (data->fd[0] != -1 && data->fd[0])
		close(data->fd[0]);
	if (data->fd[1] != -1 && data->fd[1])
		close(data->fd[1]);
	dup2(data->original_stdin, STDIN_FILENO);
	close(data->original_stdin);
	dup2(data->original_stdout, STDOUT_FILENO);
	close(data->original_stdout);
}
