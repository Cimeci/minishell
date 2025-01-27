/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/27 13:03:02 by ncharbog         ###   ########.fr       */
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

// char	**find_args(char **argv)
// {
// 	int		i;
// 	char	**args;

// 	i = 0;
// 	while (ft_strncmp(argv[i], "<<", ft_strlen(argv[i])))
// 		i++;
// 	args = malloc(sizeof(char *) * (i + 1));
// 	i = 0;
// 	while (ft_strncmp(argv[i], "<<", ft_strlen(argv[i])))
// 	{
// 		args[i] = ft_strdup(argv[i]);
// 		i++;
// 	}
// 	args[i] = NULL;
// 	return (args);
// }

// char	*ft_search_end(char **argv, int i)
// {
// 	while (argv[i] && ft_strncmp(argv[i], "<<", ft_strlen(argv[i])))
// 		i++;
// 	if (!argv[i])
// 		return (NULL);
// 	return (argv[i + 1]);
// }

// int	ft_search_index_end(char **argv, int i)
// {
// 	while (argv[i] && ft_strncmp(argv[i], "<<", ft_strlen(argv[i])))
// 		i++;
// 	if (!argv[i])
// 		return (-1);
// 	return (i);
// }

void	handle_input_loop(t_cmd *cur, t_lst *save, int i)
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
			// (*i)++;
			break ;
		}
		current = malloc(sizeof(t_lst));
		if (!current)
			exit(EXIT_FAILURE);
		current->str = ft_strdup(input);
		printf("str :%s\n", current->str);
		current->next = NULL;
		ft_lstadd_back_generic((void **)&save, current, sizeof(char *));
		free(input);
	}
}

void	execute_child_process(t_data *data,  t_cmd *cur, int pipe_fd[2])
{
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[0]);
	// int i = 0;
	// while (cur->args[i])
	// 	printf("args: %s\n", cur->args[i++]);
	execve(cur->cmd, cur->args, ft_convert_lst_to_tab(data->env));
	perror("execve");
	exit(EXIT_FAILURE);
}

void	handle_parent_process(t_lst *save, int pipe_fd[2])
{
	t_lst	*tmp;

	close(pipe_fd[0]);
	tmp = save;
	while (tmp)
	{
		dprintf(2, "%s\n", tmp->str);
		dprintf(pipe_fd[1], "%s\n", tmp->str);
		tmp = tmp->next;
	}
	close(pipe_fd[1]);
	if (wait(NULL) == -1)
		perror("wait");
}

void	process_pipe_and_fork(t_data *data, t_cmd *cur, t_lst *save)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		// free(cur->cmd);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		// free(cur->cmd);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return ;
	}
	if (pid == 0)
		execute_child_process(data, cur, pipe_fd);
	else
		handle_parent_process(save, pipe_fd);
}

void	new_args(t_cmd *cur, t_lst *save)
{
	int		i;
	char	**new;

	i = 0;
	i = ft_lstsize_generic((void *)save, sizeof(char *)) + ft_strlen_tab(&cur->args[i]);
	new = malloc((i + 1) * sizeof(char *));
	if (!new)
		return ;
	i = 0;
	while (cur->args[i])
	{
		new[i] = ft_strdup(cur->args[i]);
		i++;
	}
	while (save)
	{
		new[i] = ft_strdup(save->str);
		save = save->next;
		i++;
	}
	new[i] = 0;
	ft_free_tab(cur->args);
	cur->args = new;
}

void	process_heredoc_loop(t_data *data, t_cmd *cur)
{
	t_lst	save;
	int		i;

	i = 0;
	while (cur->heredoc[i])
	{
		//save = NULL;
		handle_input_loop(cur, &save, i);
		new_args(cur, &save);
		process_pipe_and_fork(data, cur, &save);
		ft_lstclear3(&save);
		i++;
	}
}

void	ft_heredoc(t_data *data, t_cmd *cur)
{
	if (cur->args[0][0] == '\n')
		return ;
	// printf("cur->cmd : %s\n", cur->cmd);
	process_heredoc_loop(data, cur);
}
