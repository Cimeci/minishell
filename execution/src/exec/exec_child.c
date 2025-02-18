/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:14:09 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/18 18:31:33 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_last_command(t_data *data, t_cmd *cur, int *fd)
{
	close(data->fd[1]);
	close(data->fd[0]);
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

static void	handle_pipe_command(t_data *data, t_cmd *cur, int *fd)
{
	close(data->fd[0]);
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

void	setup_child_process(t_data *data, t_cmd *cur)
{
	if (cur->here == 1)
		ft_heredoc(data, cur);
}

void	child(t_data *data, t_cmd *cur, int i)
{
	int	fd;

	fd = -1;
	if (i == data->nb_cmd - 1)
		handle_last_command(data, cur, &fd);
	else
		handle_pipe_command(data, cur, &fd);
	close_files(cur, data, fd);
	handle_commande_execution(data, cur);
	exit(EXIT_SUCCESS);
}
