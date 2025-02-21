/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/21 16:37:15 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	extension_write(t_data *data, t_cmd *cur, t_tmp_file *tmpfile)
{
	char	*input_cpy;

	input_cpy = NULL;
	if (cur->expand == true)
	{
		input_cpy = env_variables(data, ft_strdup(tmpfile->input), true);
		ft_putendl_fd(input_cpy, tmpfile->fd);
		free(input_cpy);
	}
	else
		ft_putendl_fd(tmpfile->input, tmpfile->fd);
}

static int	check_tmp_file(t_data *data, t_cmd *cur, t_tmp_file *tmpfile, int i)
{
	if (g_exit_code_sig == 130)
	{
		if (tmpfile->input)
			free(tmpfile->input);
		dup2(tmpfile->original_in, STDIN_FILENO);
		close(tmpfile->original_in);
		return (2);
	}
	if (!tmpfile->input)
		return (1);
	if (!ft_strncmp(tmpfile->input, cur->heredoc[i], ft_strlen(tmpfile->input))
		&& ft_strlen(tmpfile->input) == ft_strlen(cur->heredoc[i]))
	{
		free(tmpfile->input);
		return (1);
	}
	if (i == ft_strlen_tab(cur->heredoc) - 1)
		extension_write(data, cur, tmpfile);
	return (0);
}

static void	write_tmpfile(t_data *data, t_cmd *cur, t_tmp_file *tmpfile)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	tmpfile->original_in = dup(STDIN_FILENO);
	signal(SIGINT, child_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (cur->heredoc[i])
	{
		while (1)
		{
			tmpfile->input = readline("> ");
			error = check_tmp_file(data, cur, tmpfile, i);
			if (error == 1)
				break ;
			else if (error == 2)
				return ;
			free(tmpfile->input);
		}
		i++;
	}
	close(tmpfile->original_in);
}

static int	init_tmpfile(t_tmp_file **tmpfile)
{
	*tmpfile = malloc(sizeof(t_tmp_file));
	if (!*tmpfile)
		return (1);
	(*tmpfile)->fd = -1;
	(*tmpfile)->input = NULL;
	(*tmpfile)->original_in = -1;
	return (0);
}

void	ft_heredoc(t_data *data, t_cmd *cur)
{
	t_tmp_file	*tmpfile;

	if (init_tmpfile(&tmpfile))
		return ;
	if (cur->here == 1)
	{
		cur->file = randomizer();
		if (!cur->file)
		{
			data->gexit_code = 1;
			free(tmpfile);
			return ;
		}
		tmpfile->fd = open(cur->file, O_TRUNC | O_CREAT | O_WRONLY, 0664);
		if (tmpfile->fd < 0 || access(cur->file, F_OK) == -1)
		{
			printf("Error: heredoc's temporary file not found\n");
			free(tmpfile);
			return ;
		}
		write_tmpfile(data, cur, tmpfile);
		signal(SIGINT, SIG_DFL);
		close(tmpfile->fd);
	}
	free(tmpfile);
}
