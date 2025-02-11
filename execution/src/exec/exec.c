/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:15:03 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/11 10:45:28 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_built_in(t_data *data, t_cmd *cur)
{
	if (!cur->cmd)
		exit(0);
	if (!ft_strncmp(cur->cmd, "exit", ft_strlen(cur->cmd))
		&& ft_strlen(cur->cmd) == 4)
		return (1);
	if (!ft_strncmp(cur->cmd, "cd", ft_strlen(cur->cmd))
		&& ft_strlen(cur->cmd) == 2)
		ft_cd(data, cur);
	else if (!ft_strncmp(cur->args[0], "pwd", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 3)
		ft_pwd(data);
	else if (!ft_strncmp(cur->args[0], "echo", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 4)
		ft_echo(cur->args);
	else if (!ft_strncmp(cur->args[0], "env", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 3)
		ft_env(data, cur);
	else if (!ft_strncmp(cur->args[0], "export", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 6)
		ft_export(data, cur);
	else if (!ft_strncmp(cur->args[0], "unset", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 5)
		ft_unset(data, cur);
	else
		return (0);
	return (1);
}

void	close_files(t_cmd *cur)
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
}

void	parent(t_data *data)
{
	close(data->fd[1]);
	dup2(data->fd[0], STDIN_FILENO);
	close(data->fd[0]);
}

void	files(t_data *data, t_cmd *cur)
{
	int	i;
	int	type;

	i = 0;
	type = 0;
	if (cur->here == 1)
		cur->file = randomizer();
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
		{
			if (cur->flag_redir[type] == 1)
				cur->fd_outfile = open(cur->outfile[i],
					O_CREAT | O_WRONLY | O_TRUNC, 0644);
			else if (cur->flag_redir[type] == 2)
				cur->fd_outfile = open(cur->outfile[i],
					O_CREAT | O_WRONLY | O_APPEND, 0644);
		}
		if (cur->fd_outfile < 0)
			errors(data, cur->outfile[i], ERRNO);
		}
		i++;
		type++;
}

void	child(t_data *data, t_cmd *cur, int i)
{
	if (cur->here == 1)
		ft_heredoc(data, cur);
	if (i == data->nb_cmd - 1)
	{
		close(data->fd[1]);
		close(data->fd[0]);
		if (cur->infile)
			dup2(cur->fd_infile, STDIN_FILENO);
		if (cur->outfile)
			dup2(cur->fd_outfile, STDOUT_FILENO);
	}
	else
	{
		close(data->fd[0]);
		if (cur->infile)
			dup2(cur->fd_infile, STDIN_FILENO);
		if (!cur->outfile)
			dup2(data->fd[1], STDOUT_FILENO);
		else
			dup2(cur->fd_outfile, STDOUT_FILENO);
		close(data->fd[1]);
	}
	close_files(cur);
	if (!exec_built_in(data, cur))
	{
		if (execve(cur->cmd, cur->args, data->env_cp) == -1)
		{
			if (opendir(cur->cmd) != NULL)
			{
				errors(data, cur->cmd, DIRECTORY);
				exit(126);
			}
			if (access(cur->cmd, X_OK) != 0)
			{
				errors(data, cur->cmd, CMD_NOT_FOUND);
				exit(127);
			}
		}
	}
	exit(EXIT_SUCCESS);
}

void	exec(t_data *data)
{
	int		i;
	t_cmd	*cur;
	pid_t	p;
	int		status;

	if (!data->cmd)
		return ;
	i = 0;
	cur = data->cmd;
	data->original_stdin = dup(STDIN_FILENO);
	data->original_stdout = dup(STDOUT_FILENO);
	signal(SIGINT, SIG_IGN);
	data->nb_cmd = ft_lstsize_generic((void *)cur, sizeof(t_cmd) - sizeof(t_cmd *));
	if (data->nb_cmd == 1)
	{
		if (cur->cmd && !ft_strncmp(cur->cmd, "exit", ft_strlen(cur->cmd))
			&& ft_strlen(cur->cmd) == 4)
		{
			ft_exit(data, cur);
			i++;
		}
		if (cur->cmd && !ft_strncmp(cur->cmd, "cd", ft_strlen(cur->cmd))
			&& ft_strlen(cur->cmd) == 2)
		{
			ft_cd(data, cur);
			i++;
		}
		else if (cur->cmd && !ft_strncmp(cur->args[0], "export", ft_strlen(cur->args[0]))
			&& ft_strlen(cur->args[0]) == 6)
		{
			ft_export(data, cur);
			i++;
		}
		else if (cur->cmd && !ft_strncmp(cur->args[0], "unset", ft_strlen(cur->args[0]))
			&& ft_strlen(cur->args[0]) == 5)
		{
			ft_unset(data, cur);
			i++;
		}
	}
	while (cur && i < data->nb_cmd)
	{
		files(data, cur);
		if (cur->here == 1)
			cur->file = randomizer();
		if (pipe(data->fd) == -1)
			printf("pipe failed\n");
		p = fork();
		if (p < 0)
			printf("fork failed\n");
		else if (p == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (cur->fd_infile == -1 || cur->fd_outfile == -1)
				exit(1);
			child(data, cur, i);
		}
		else
			parent(data);
		i++;
		cur = cur->next;
	}
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			data->gexit_code = WEXITSTATUS(status);
	}
	signal(SIGINT, parent_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	close(data->fd[0]);
	close(data->fd[1]);
	dup2(data->original_stdin, STDIN_FILENO);
	close(data->original_stdin);
	dup2(data->original_stdout, STDOUT_FILENO);
	close(data->original_stdout);
}
