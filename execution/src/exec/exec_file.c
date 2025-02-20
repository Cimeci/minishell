/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:58:21 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/20 10:42:09 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		errors(data, cur->infile[i], FILES);
		data->gexit_code = 1;
		return (true);
	}
	return (false);
}

int	files(t_data *data, t_cmd *cur)
{
	int		i;
	int		fd;
	int		type;
	bool	error;

	i = 0;
	type = 0;
	error = false;
	while (cur->infile && cur->infile[i])
	{
		if (i == ft_strlen_tab(cur->infile) - 1)
			cur->fd_infile = open(cur->infile[i], O_RDONLY);
		fd = open(cur->infile[i], O_RDONLY);
		if (fd < 0)
		{
			errors(data, cur->infile[i], FILES);
			data->gexit_code = 1;
			error = true;
			break ;
		}
		if (i < ft_strlen_tab(cur->infile) - 1)
			close(fd);
		i++;
	}
	i = 0;
	while (cur->outfile && cur->outfile[i])
	{
		if (i == ft_strlen_tab(cur->outfile) - 1)
			error = open_redir(data, cur, type, i);
		else if (cur->flag_redir[type] == 1)
		{
			fd = open(cur->outfile[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd < 0)
			{
				errors(data, cur->outfile[i], ERRNO);
				data->gexit_code = 1;
				error = true;
				break ;
			}
		}
		else if (cur->flag_redir[type] == 2 && open(cur->outfile[i],
				O_CREAT | O_WRONLY | O_APPEND, 0644) < 0)
		{
			fd = open(cur->outfile[i], O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (fd < 0)
			{
				errors(data, cur->outfile[i], ERRNO);
				data->gexit_code = 1;
				error = true;
				break ;
			}
		}
		if (i < ft_strlen_tab(cur->outfile) - 1)
			close(fd);
		i++;
		type++;
	}
	return (error);
}
