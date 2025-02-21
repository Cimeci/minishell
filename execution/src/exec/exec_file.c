/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:58:21 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/21 15:07:06 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_files(t_cmd *cur, t_data *data, int fd, bool child)
{
	if (cur->fd_infile != -1 && cur->fd_infile)
	{
		close(cur->fd_infile);
		cur->fd_infile = -1;
	}
	if (cur->fd_outfile != -1 && cur->fd_outfile)
	{
		close(cur->fd_outfile);
		cur->fd_outfile = -1;
	}
	if (fd != -1 && fd)
		close(fd);
	if (child == true)
	{
		close(data->original_stdin);
		close(data->original_stdout);
	}
}

bool	open_redir(t_data *data, t_cmd *cur, int type, int i)
{
	if (cur->flag_redir[type] == 1)
		cur->fd_outfile = open(cur->outfile[i], O_CREAT | O_WRONLY | O_TRUNC,
				0644);
	else if (cur->flag_redir[type] == 2)
		cur->fd_outfile = open(cur->outfile[i], O_CREAT | O_WRONLY | O_APPEND,
				0644);
	if (cur->fd_outfile < 0)
	{
		errors(data, cur->outfile[i], ERRNO);
		data->gexit_code = 1;
		return (true);
	}
	return (false);
}
