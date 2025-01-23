/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/23 11:02:25 by inowak--         ###   ########.fr       */
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

char	**find_args(char **argv)
{
	int		i;
	char	**args;

	i = 0;
	while (ft_strncmp(argv[i], "<<", ft_strlen(argv[i])))
		i++;
	args = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (ft_strncmp(argv[i], "<<", ft_strlen(argv[i])))
	{
		args[i] = ft_strdup(argv[i]);
		i++;
	}
	args[i] = NULL;
	return (args);
}

char	*ft_search_end(char **argv, int i)
{
	while (argv[i] && ft_strncmp(argv[i], "<<", ft_strlen(argv[i])))
		i++;
	if (!argv[i])
		return (NULL);
	return (argv[i + 1]);
}

int	ft_search_index_end(char **argv, int i)
{
	while (argv[i] && ft_strncmp(argv[i], "<<", ft_strlen(argv[i])))
		i++;
	if (!argv[i])
		return (-1);
	return (i);
}

void	handle_invalid_command(char *path)
{
	fprintf(stderr, "Error: invalid arguments or command\n");
	free(path);
}

void	handle_input_loop(char **argv, t_lst **save, int *i)
{
	char	*input;
	char	*end;
	t_lst	*cur;

	while (1)
	{
		input = readline("> ");
		*i = ft_search_index_end(argv, *i);
		if (*i == -1)
			return ;
		end = ft_search_end(argv, *i);
		if (!end || !input || !ft_strncmp(input, end, ft_strlen(end)))
		{
			free(input);
			(*i)++;
			break ;
		}
		cur = malloc(sizeof(t_lst));
		if (!cur)
			exit(EXIT_FAILURE);
		cur->str = ft_strdup(input);
		printf("str :%s\n", cur->str);
		cur->next = NULL;
		ft_lstadd_back_generic((void **)save, cur, sizeof(t_lst));
		free(input);
	}
}

void	execute_child_process(char *path, char **argv, char **env,
		int pipe_fd[2])
{
	char	**args;

	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[0]);
	args = find_args(argv);
	if (!args)
		exit(EXIT_FAILURE);
	int i = 0;
	while (args[i])
		printf("args: %s\n", args[i++]);
	execve(path, args, env);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	handle_parent_process(t_lst *save, int pipe_fd[2])
{
	t_lst	*tmp;

	tmp = save;
	close(pipe_fd[0]);
	while (tmp)
	{
		dprintf(pipe_fd[1], "%s\n", tmp->str);
		tmp = tmp->next;
	}
	close(pipe_fd[1]);
	if (wait(NULL) == -1)
		perror("wait");
}

void	process_pipe_and_fork(char *path, char **argv, char **env, t_lst *save)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		free(path);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(path);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return ;
	}
	if (pid == 0)
		execute_child_process(path, argv, env, pipe_fd);
	else
		handle_parent_process(save, pipe_fd);
}

void	process_heredoc_loop(char **argv, char **env, char *path)
{
	t_lst	*save;
	int		i;

	i = 0;
	while (ft_search_index_end(argv, i) != -1)
	{
		save = NULL;
		handle_input_loop(argv, &save, &i);
		if (ft_search_index_end(argv, i) == -1)
			process_pipe_and_fork(path, argv, env, save);
		ft_lstclear3(save);
	}
}

void	ft_heredoc(t_data *data, t_cmd *cur)
{
	if (cur->args[0][0] == '\n')
		return ;
	printf("cur->cmd : %s\n", cur->cmd);
	process_heredoc_loop(cur->args, ft_convert_lst_to_tab(data->env), cur->cmd);
}

