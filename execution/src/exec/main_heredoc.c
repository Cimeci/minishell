/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/27 16:25:24 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstclear3(t_lst *save)
{
	t_lst	*temp;

	temp = save;
	while (save)
	{
		temp = save->next;
		if (save->str)
			free(save->str);
		free(save);
		save = temp;
	}
}

t_lst	*ft_init_save(void)
{
	t_lst	*save;

	save = malloc(sizeof(t_lst));
	return (save);
}

void	handle_input_loop(t_cmd *cur, t_lst **save, int i)
{
	char	*input;
	char	*end;
	t_lst	*current;

	while (1)
	{
		input = readline("> ");
		end = cur->heredoc[i];
		if (!end || !input || (!ft_strncmp(input, end, ft_strlen(end))
			&& ft_strlen(end) == ft_strlen(input)))
		{
			free(input);
			break ;
		}
		current = malloc(sizeof(t_lst));
		if (!current)
			exit(EXIT_FAILURE);
		current->str = ft_strdup(input);
		current->next = NULL;
		ft_lstadd_back_generic((void **)save, current, sizeof(char *));
		free(input);
	}
}

void	execute_child_process(t_data *data,  t_cmd *cur, int pipe_fd[2])
{
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[0]);
	if (cur->fd_outfile > 0)
	{
		dprintf(2, "LA YA UN OUTFILE\n");
		dup2(cur->fd_outfile, STDOUT_FILENO);
		close(cur->fd_outfile);
		cur->fd_outfile = -1;
	}
	else if (cur->next && cur->fd_outfile < 0)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	// dprintf(2, "Here\n");
	if (cur->next == NULL)
	{
		execve(cur->cmd, cur->args, ft_convert_lst_to_tab(data->env));
		perror("execve");
	}
	exit(EXIT_FAILURE);
}

void	handle_parent_process(t_lst *save, int pipe_fd[2])
{
	close(pipe_fd[0]);
	while (save)
	{
		dprintf(pipe_fd[1], "%s\n", save->str);
		save = save->next;
	}
	close(pipe_fd[1]);
	if (wait(NULL) == -1)
		perror("wait");
}

int	process_pipe_and_fork(t_data *data, t_cmd *cur, t_lst *save)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (0);
	}
	if (pid == 0)
		execute_child_process(data, cur, pipe_fd);
	else
		handle_parent_process(save, pipe_fd);
	int input_fd;
	input_fd = dup(pipe_fd[0]);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (input_fd);
}

int	process_heredoc_loop(t_data *data, t_cmd *cur)
{
	t_lst	*save;
	int		i;
	int input_fd;

	i = 0;
	while (cur->heredoc[i])
	{
		save = NULL;
		handle_input_loop(cur, &save, i);
		if (ft_strlen_tab(cur->heredoc) - 1 == i)
			input_fd = process_pipe_and_fork(data, cur, save);
		ft_lstclear3(save);
		i++;
	}
	return (input_fd);
}

int	ft_heredoc(t_data *data, t_cmd *cur)
{
	int input_fd;
	if (cur->args[0][0] == '\n')
		return (0);
    input_fd = process_heredoc_loop(data, cur);
	return (input_fd);
}
