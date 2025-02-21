/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:28:59 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/21 16:32:09 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_fd_outfile(t_data *data, char *file_to_open, t_file *file)
{
	if (file->fd_o < 0)
	{
		errors(data, file_to_open, ERRNO);
		data->gexit_code = 1;
		file->error = true;
		return (1);
	}
	return (0);
}

static int	check_fd_infile(t_data *data, int fd, char *file_to_open, t_file *file)
{
	file->fd_i = fd;
	if (file->fd_i < 0)
	{
		errors(data, file_to_open, ERRNO);
		data->gexit_code = 1;
		file->error = true;
		return (1);
	}
	return (0);
}

static void	infile(t_data *data, t_cmd *cur, t_file *file)
{
	int	i;

	i = 0;
	while (cur->infile && cur->infile[i])
	{
		if (i == ft_strlen_tab(cur->infile) - 1)
		{
			cur->fd_infile = open(cur->infile[i], O_RDONLY);
			if (check_fd_infile(data, cur->fd_infile, cur->infile[i], file))
				break ;
		}
		else
		{
			file->fd_i = open(cur->infile[i], O_RDONLY);
			if (check_fd_infile(data, file->fd_i, cur->infile[i], file))
				break ;
		}
		if (i < ft_strlen_tab(cur->infile) - 1)
			close(file->fd_i);
		i++;
	}
}

static void	outfile(t_data *data, t_cmd *cur, t_file *f)
{
	int	i;

	i = 0;
	while (cur->outfile && cur->outfile[i])
	{
		if (i == ft_strlen_tab(cur->outfile) - 1)
			f->error = open_redir(data, cur, f->type, i);
		else if (cur->flag_redir[f->type] == 1)
		{
			f->fd_o = open(cur->outfile[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (check_fd_outfile(data, cur->outfile[i], f))
				break ;
		}
		else if (cur->flag_redir[f->type] == 2)
		{
			f->fd_o = open(cur->outfile[i], O_CREAT | O_WRONLY | O_APPEND,
					0644);
			if (check_fd_outfile(data, cur->outfile[i], f))
				break ;
		}
		if (i < ft_strlen_tab(cur->outfile) - 1)
			close(f->fd_o);
		i++;
		f->type++;
	}
}

int	files(t_data *data, t_cmd *cur)
{
	t_file	*file;
	bool	error;

	error = false;
	file = malloc(sizeof(t_file));
	file->error = false;
	file->fd_o = -1;
	file->fd_i = -1;
	file->type = 0;
	infile(data, cur, file);
	outfile(data, cur, file);
	if (error != file->error)
		error = file->error;
	free(file);
	return (error);
}
