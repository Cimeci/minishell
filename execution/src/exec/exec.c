/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:15:03 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/11 18:51:12 by inowak--         ###   ########.fr       */
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
		return (1);
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
		return (1);
	else if (!ft_strncmp(cur->args[0], "unset", ft_strlen(cur->args[0]))
		&& ft_strlen(cur->args[0]) == 5)
		return (1);
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

void	handle_commande_execution(t_data *data, t_cmd *cur)
{
	if (!exec_built_in(data, cur))
	{
		if (execve(cur->cmd, cur->args, ft_convert_lst_to_tab(data->env)) == -1)
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
}

void	child(t_data *data, t_cmd *cur, int i)
{
	int	fd;

	if (cur->here == 1)
		ft_heredoc(data, cur);
	if (i == data->nb_cmd - 1)
	{
		close(data->fd[1]);
		close(data->fd[0]);
		if (cur->infile || cur->here == 1)
		{
			if (cur->here == 1)
			{
				fd = open(cur->file, O_RDONLY);
				dup2(fd, STDIN_FILENO);
			}
			else
				dup2(cur->fd_infile, STDIN_FILENO);
		}
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
	// dprintf(2, "%d|%d\n", cur->fd_infile, cur->fd_outfile);
	close_files(cur);
	handle_commande_execution(data, cur);
	// if (cur->here == 1)
	// 	close(fd);
	exit(EXIT_SUCCESS);
}

void	handle_unique_builtin(t_data *data, t_cmd *cur)
{
	if (data->nb_cmd == 1)
	{
		if (cur->cmd && !ft_strncmp(cur->cmd, "exit", ft_strlen(cur->cmd))
			&& ft_strlen(cur->cmd) == 4)
			ft_exit(data, cur);
		else if (cur->cmd && !ft_strncmp(cur->cmd, "cd", ft_strlen(cur->cmd))
			&& ft_strlen(cur->cmd) == 2)
			ft_cd(data, cur);
		else if (cur->cmd && !ft_strncmp(cur->args[0], "export",
				ft_strlen(cur->args[0])) && ft_strlen(cur->args[0]) == 6)
			ft_export(data, cur);
		else if (cur->cmd && !ft_strncmp(cur->args[0], "unset",
				ft_strlen(cur->args[0])) && ft_strlen(cur->args[0]) == 5)
			ft_unset(data, cur);
	}
}

void	setup_execution(t_data *data)
{
	data->original_stdin = dup(STDIN_FILENO);
	data->original_stdout = dup(STDOUT_FILENO);
	signal(SIGINT, parent_signal_handler_exec);
	data->nb_cmd = ft_lstsize_generic((void *)data->cmd, sizeof(t_cmd)
			- sizeof(t_cmd *));
	handle_unique_builtin(data, data->cmd);
}

int	handle_here_doc(t_data *data, t_cmd *cur)
{
	if (cur->here == 1)
	{
		cur->file = randomizer();
		if (!cur->file)
		{
			data->gexit_code = 1;
			return (0);
		}
	}
	return (1);
}

void	execute_pipeline(t_data *data)
{
	int		i;
	t_cmd	*cur;
	pid_t	p;

	i = 0;
	cur = data->cmd;
	while (cur && i < data->nb_cmd)
	{
		if (!handle_here_doc(data, cur))
			return ;
		files(data, cur);
		if (pipe(data->fd) == -1)
			printf("pipe failed\n");
		p = fork();
		if (p < 0)
			printf("fork failed\n");
		else if (p == 0)
			child(data, cur, i);
		else
			parent(data);
		i++;
		cur = cur->next;
	}
}

void	handle_parent_process(t_data *data)
{
	int	status;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status) && data->gexit_code == 0)
			data->gexit_code = WEXITSTATUS(status);
	}
	signal(SIGINT, parent_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	cleanup_execution(t_data *data)
{
	close(data->fd[0]);
	close(data->fd[1]);
	dup2(data->original_stdin, STDIN_FILENO);
	close(data->original_stdin);
	dup2(data->original_stdout, STDOUT_FILENO);
	close(data->original_stdout);
}

void	exec(t_data *data)
{
	if (!data->cmd)
		return ;
	setup_execution(data);
	execute_pipeline(data);
	handle_parent_process(data);
	cleanup_execution(data);
}
