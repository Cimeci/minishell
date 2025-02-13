/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:58:21 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/13 17:16:58 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_files(t_cmd *cur, t_data *data, int fd)
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
	close(fd);
	close(data->original_stdin);
	close(data->original_stdout);
}

void	open_redir(t_cmd *cur, int type, int i)
{
	if (cur->flag_redir[type] == 1)
		cur->fd_outfile = open(cur->outfile[i], O_CREAT | O_WRONLY | O_TRUNC,
				0644);
	else if (cur->flag_redir[type] == 2)
		cur->fd_outfile = open(cur->outfile[i], O_CREAT | O_WRONLY | O_APPEND,
				0644);
}

void	files(t_data *data, t_cmd *cur)
{
	int	i;
	int	type;

	i = 0;
	type = 0;
	while (cur->infile && cur->infile[i])
	{
		if (i == ft_strlen_tab(cur->infile) - 1)
			cur->fd_infile = open(cur->infile[i], O_RDONLY);
		if (open(cur->infile[i], O_RDONLY) < 0)
			errors(data, cur->infile[i], FILES);
		i++;
	}
	i = 0;
	while (cur->outfile && cur->outfile[i])
	{
		if (i == ft_strlen_tab(cur->outfile) - 1)
			open_redir(cur, type, i);
		if (cur->fd_outfile < 0)
			errors(data, cur->outfile[i], ERRNO);
		i++;
		type++;
	}
}
