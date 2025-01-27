/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:15:03 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/27 16:16:27 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	incorrect_infile(void)
{
	int	null;

	null = open("/dev/null", O_RDONLY);
	if (null != -1)
	{
		dup2(null, STDIN_FILENO);
		close(null);
	}
}

void	incorrect_outfile(void)
{
	int	null;

	null = open("/dev/null", O_WRONLY);
	if (null != -1)
	{
		dup2(null, STDOUT_FILENO);
		close(null);
	}
}

int	is_built_in(t_data *data, t_cmd *cur)
{
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

void	files(t_cmd *cur)
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
			printf("%s n'existe pas ou n'a pas les droits", cur->infile[i]);
		i++;
	}
	i = 0;
	while (cur->outfile && cur->outfile[i])
	{
		if (cur->flag_redir[type] == 1)
		{
			if (i == ft_strlen_tab(cur->outfile) - 1)
				cur->fd_outfile = open(cur->outfile[i], O_CREAT | O_WRONLY | O_TRUNC,
						0644);
			if (open(cur->outfile[i], O_CREAT | O_WRONLY | O_TRUNC, 0644) < 0)
			{
				if (errno == EACCES)
					printf("outfile n'a pas les droits\n");
			}
		}
		else if (cur->flag_redir[type] == 2)
		{
			if (i == ft_strlen_tab(cur->outfile) - 1)
				cur->fd_outfile = open(cur->outfile[i], O_CREAT | O_WRONLY | O_APPEND,
						0644);
			if (open(cur->outfile[i], O_CREAT | O_WRONLY | O_APPEND, 0644) < 0)
			{
				if (errno == EACCES)
					printf("outfile n'a pas les droits\n");
			}
		}
		i++;
		type++;
	}
}

void	unique_cmd(t_data *data, t_cmd *cur)
{
	pid_t pid;

	files(cur);
	if (cur->here == 1)
	{
		ft_heredoc(data, cur);
		return ;
	}
	if (cur->fd_outfile)
		dup2(cur->fd_outfile, STDOUT_FILENO);
	if (cur->fd_infile)
		dup2(cur->fd_infile, STDIN_FILENO);
	close_files(cur);
	if (!is_built_in(data, cur))
	{
		if (cur->cmd && access(cur->cmd, X_OK) == 0)
		{
			pid = fork();
			if (pid == -1)
				exit(EXIT_FAILURE);
			else if (pid == 0)
				execve(cur->cmd, cur->args, ft_convert_lst_to_tab(data->env));
			else if (wait(NULL) == -1)
				perror("wait failed");
		}
	}
}

void	child(t_data *data, t_cmd *cur, int i)
{
	int	input_fd;

	input_fd = -1;
	if (cur->here == 1)
	{
		input_fd = ft_heredoc(data, cur);
		dup2(input_fd, STDOUT_FILENO);
		close(input_fd);
		exit(0);
	}
	if (i == 0)
	{
		close(data->fd[0]);
		if (!cur->outfile && data->nb_cmd > 1)
			dup2(data->fd[1], STDOUT_FILENO);
		else if (cur->outfile)
			dup2(cur->fd_outfile, STDOUT_FILENO);
		if (cur->fd_infile == -1)
			incorrect_infile();
		else
			dup2(cur->fd_infile, STDIN_FILENO);
		close(data->fd[1]);
	}
	else if (i == data->nb_cmd - 1)
	{
		close(data->fd[1]);
		close(data->fd[0]);
		if (cur->infile)
			dup2(cur->fd_infile, STDIN_FILENO);
		if (cur->fd_outfile == -1)
			incorrect_outfile();
		else if (cur->outfile)
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
	// if (cur->here == true)
	// 	exit (0);
	if (!is_built_in(data, cur))
	{
		if (execve(cur->cmd, cur->args, ft_convert_lst_to_tab(data->env)) == -1)
		{
			perror("Error");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}

void exec(t_data *data)
{
	int		i;
	t_cmd	*cur;
	pid_t	p;

	if (!data->cmd)
		return ;
	data->original_stdin = dup(STDIN_FILENO);
	data->original_stdout = dup(STDOUT_FILENO);
	cur = data->cmd;
	i = 0;
	if (!cur->next)
		unique_cmd(data, cur);
	else
	{
		data->nb_cmd = ft_lstsize_generic((void *)cur, sizeof(t_cmd) - sizeof(t_cmd *));
		while (cur && i < data->nb_cmd)
		{
			files(cur);
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
		while (wait(NULL) != -1)
			;
		close(data->fd[0]);
		close(data->fd[1]);
	}
	dup2(data->original_stdin, STDIN_FILENO);
	close(data->original_stdin);
	dup2(data->original_stdout, STDOUT_FILENO);
	close(data->original_stdout);
}
