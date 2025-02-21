/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parent.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:08:15 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/21 14:45:05 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parent(t_data *data)
{
	close(data->fd[1]);
	dup2(data->fd[0], STDIN_FILENO);
	close(data->fd[0]);
}

void    handle_parent_process(t_data *data, t_cmd *last)
{
	int status;
	int last_error;

	last_error = 0;
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) != 0)
				last_error = WEXITSTATUS(status);
			else if (last_error == 0)
				data->gexit_code = WEXITSTATUS(status);
		}
	}
	if (last_error != 0)
		data->gexit_code = last_error;
	if ((last->infile && access(last->infile[ft_strlen_tab(last->infile) -1], R_OK) != 0)
		|| (last->outfile && access(last->outfile[ft_strlen_tab(last->outfile) -1], W_OK) != 0))
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
