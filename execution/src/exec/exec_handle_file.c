/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:28:59 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/20 19:00:15 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	check_fd_file(t_data *data, char *file, int fd, bool **error)
{
	if (fd < 0)
	{
		errors(data, file, ERRNO);
		data->gexit_code = 1;
		**error = true;
		return (false);
	}
	return (**error);
}

void	infile(t_data *data, t_cmd *cur, int fd, bool *error)
{
	int	i;

	i = 0;
	while (cur->infile && cur->infile[i])
	{
		if (i == ft_strlen_tab(cur->infile) - 1)
		{
			cur->fd_infile = open(cur->infile[i], O_RDONLY);
			if (check_fd_file(data, cur->infile[i], fd, &error) == false)
				break ;
		}
		else
		{
			fd = open(cur->infile[i], O_RDONLY);
			if (check_fd_file(data, cur->infile[i], fd, &error) == false)
				break ;
		}
		if (i < ft_strlen_tab(cur->infile) - 1)
			close(fd);
		i++;
	}
}

void	outfile(t_data *data, t_cmd *cur, int fd, bool *error)
{
	int	type;
	int	i;

	type = 0;
	i = 0;
	while (cur->outfile && cur->outfile[i])
	{
		if (i == ft_strlen_tab(cur->outfile) - 1)
			*error = open_redir(data, cur, type, i);
		else if (cur->flag_redir[type] == 1)
		{
			fd = open(cur->outfile[i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (check_fd_file(data, cur->outfile[i], fd, &error) == false)
				break ;
		}
		else if (cur->flag_redir[type] == 2 && open(cur->outfile[i],
				O_CREAT | O_WRONLY | O_APPEND, 0644) < 0)
		{
			fd = open(cur->outfile[i], O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (check_fd_file(data, cur->outfile[i], fd, &error) == false)
				break ;
		}
		if (i < ft_strlen_tab(cur->outfile) - 1)
			close(fd);
		i++;
		type++;
	}
}

int	files(t_data *data, t_cmd *cur)
{
	int		fd;
	bool	error;

	fd = -1;
	error = false;
	infile(data, cur, fd, &error);
	outfile(data, cur, fd, &error);
	printf("error : %d\n", error);
	return (error);
}
