/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:14:09 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/26 11:10:43 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	empty_pipe(void)
{
	int	null;

	null = open("/dev/null", O_RDONLY);
	if (null != -1)
	{
		dup2(null, STDIN_FILENO);
		close(null);
	}
}

static void	handle_last_command(t_data *data, t_cmd *cur, int *fd,
		bool prev_cmd)
{
	close(data->fd[1]);
	close(data->fd[0]);
	if (prev_cmd == false)
		empty_pipe();
	if (cur->infile || cur->here == 1)
	{
		if (cur->here == 1)
		{
			*fd = open(cur->file, O_RDONLY);
			unlink(cur->file);
			dup2(*fd, STDIN_FILENO);
		}
		else
			dup2(cur->fd_infile, STDIN_FILENO);
	}
	if (cur->outfile)
		dup2(cur->fd_outfile, STDOUT_FILENO);
}

static void	handle_pipe_command(t_data *data, t_cmd *cur, int *fd,
		bool prev_cmd)
{
	close(data->fd[0]);
	if (prev_cmd == false)
		empty_pipe();
	if (cur->infile || cur->here == 1)
	{
		if (cur->here == 1)
		{
			*fd = open(cur->file, O_RDONLY);
			unlink(cur->file);
			dup2(*fd, STDIN_FILENO);
		}
		else
			dup2(cur->fd_infile, STDIN_FILENO);
	}
	if (!cur->outfile)
		dup2(data->fd[1], STDOUT_FILENO);
	else
		dup2(cur->fd_outfile, STDOUT_FILENO);
	close(data->fd[1]);
}

void	child(t_data *data, t_cmd *cur, int i, bool prev_cmd)
{
	int	fd;

	fd = -1;
	if (i == data->nb_cmd - 1)
		handle_last_command(data, cur, &fd, prev_cmd);
	else
		handle_pipe_command(data, cur, &fd, prev_cmd);
	close_files(cur, data, fd, true);
	handle_command_execution(data, cur);
	exit(EXIT_SUCCESS);
}
