/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parent.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:08:15 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/26 13:33:40 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent(t_data *data)
{
	close(data->fd[1]);
	dup2(data->fd[0], STDIN_FILENO);
	close(data->fd[0]);
}

void	check_signal(t_data *data, int status)
{
	data->gexit_code = WTERMSIG(status);
	if (data->gexit_code == 3)
		parent_signal_handler_exec(SIGQUIT);
	else if (data->gexit_code == 2)
		parent_signal_handler_exec(SIGINT);
	else if (data->gexit_code == 13)
		data->gexit_code = 0;
}

int	check_exit(t_data *data, int status, int last_error)
{
	if (WEXITSTATUS(status) != 0)
		last_error = WEXITSTATUS(status);
	else if (last_error == 0)
		data->gexit_code = WEXITSTATUS(status);
	return (last_error);
}

void	handle_parent_process(t_data *data, t_cmd *last)
{
	int	status;
	int	last_error;

	last_error = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			last_error = check_exit(data, status, last_error);
		if (WIFSIGNALED(status))
			check_signal(data, status);
	}
	if (last_error != 0)
		data->gexit_code = last_error;
	if ((last->infile && access(last->infile[ft_strlen_tab(last->infile) - 1],
				R_OK) != 0) || (last->outfile
			&& access(last->outfile[ft_strlen_tab(last->outfile) - 1],
				W_OK) != 0))
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
