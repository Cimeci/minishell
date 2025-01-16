/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/16 16:25:50 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	while (table[i])
		free(table[i++]);
	free(table);
	table = NULL;
}

char	*my_getenv(const char *name, char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] != '=' && env[i][j])
			j++;
		tmp = ft_substr(env[i], 0, j);
		if (ft_strncmp(tmp, name, j) == 0 && name[j] == '\0')
		{
			free(tmp);
			return (env[i] + j + 1);
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

char	*find_pathname(char *cmd, char **cmd_split, char **path_split, int i)
{
	char	*pathname;

	cmd = ft_strjoin("/", cmd_split[0]);
	pathname = ft_strjoin(path_split[i], cmd);
	free(cmd);
	if (!access(pathname, F_OK | X_OK))
	{
		ft_free_tab(path_split);
		ft_free_tab(cmd_split);
		return (pathname);
	}
	free(pathname);
	return (NULL);
}

char	*find_path(char **env, char *cmd)
{
	char	**cmd_split;
	char	**path_split;
	char	*pathname;
	char	*tmp;
	int		i;

	i = 0;
	tmp = my_getenv("PATH", env);
	if (!tmp)
		return (NULL);
	if (ft_strncmp(cmd, "./", 2) == 0 || !access(cmd, X_OK))
		return (cmd);
	path_split = ft_split(tmp, ':');
	cmd_split = ft_split(cmd, ' ');
	while (path_split[i])
	{
		pathname = find_pathname(cmd, cmd_split, path_split, i);
		if (pathname)
			return (pathname);
		i++;
	}
	ft_free_tab(path_split);
	ft_free_tab(cmd_split);
	return (NULL);
}

int	ft_lstsize3(t_save *lst)
{
	int	i;

	i = 0;
	if (!lst)
		return (0);
	if (lst->next == NULL)
		return (1);
	while (lst->next != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i + 1);
}

char	**ft_convert_lst_to_tab2(t_save *save)
{
	int		i;
	char	**table;
	t_save	*tmp;

	i = 0;
	table = malloc(sizeof(char *) * (ft_lstsize3(save) + 1));
	if (!table)
		return (NULL);
	tmp = save;
	while (tmp)
	{
		table[i] = ft_strdup(tmp->line);
		i++;
		tmp = tmp->next;
	}
	table[i] = NULL;
	return (table);
}

void	ft_lstadd_back3(t_save **lst, t_save *new)
{
	t_save	*temp;

	if (!lst || !new)
		return ;
	temp = *lst;
	if (!temp)
	{
		*lst = new;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

void	ft_lstclear3(t_save *save)
{
	t_save	*temp;

	temp = save;
	while (save)
	{
		temp = save->next;
		if (save->line)
			free(save->line);
		free(save);
		save = temp;
	}
}

t_save	*ft_init_save(void)
{
	t_save	*save;

	save = malloc(sizeof(t_save));
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

char	*ft_search_end(char **argv)
{
	int	i;

	i = 0;
	while (argv[i] && ft_strncmp(argv[i], "<<", ft_strlen(argv[i])))
		i++;
	if (!argv[i])
		return (NULL);
	return (argv[i + 1]);
}

void	execution_cmd(char **argv, char **env)
{
	char	*path;
	pid_t	pid;

	if (!env)
		return ;
	path = find_path(env, argv[0]);
	if (!path)
		return ;
	if (path && access(path, X_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		else if (pid == 0)
			execve(path, argv, env);
		else if (wait(NULL) == -1)
			perror("wait failed");
	}
	// ft_free_tab(env);
	free(path);
}

void	ft_heredoc(char **argv, char **env)
{
	char	*input;
	t_save	*save;
	t_save	*cur;
	pid_t	pid;
	int		pipe_fd[2];
	char	*path;
	char	**args;
	t_save	*tmp;
	t_save	*next;
	char	*end;

	if (argv[0][0] == '\n')
		return ;
	if (!ft_search_end(argv))
	{
		execution_cmd(argv, env);
		return ;
	}
	path = find_path(env, argv[0]);
	if (!path || access(path, X_OK))
	{
		fprintf(stderr, "Error: invalid arguments or command\n");
		free(path);
		return ;
	}
	save = NULL;
	while (1)
	{
		input = readline("> ");
		end = ft_search_end(argv);
		if (!end)
			return ;
		if (!input || !ft_strncmp(input, end, ft_strlen(end)))
		{
			free(input);
			break ;
		}
		cur = malloc(sizeof(t_save));
		if (!cur)
			exit(EXIT_FAILURE);
		cur->line = strdup(input);
		cur->next = NULL;
		ft_lstadd_back3(&save, cur);
		free(input);
	}
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
	else if (pid == 0)
	{
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
		execve(path, args, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(pipe_fd[0]);
		tmp = save;
		while (tmp)
		{
			dprintf(pipe_fd[1], "%s\n", tmp->line);
			tmp = tmp->next;
		}
		close(pipe_fd[1]);
		if (wait(NULL) == -1)
			perror("wait");
	}
	free(path);
	while (save)
	{
		next = save->next;
		free(save->line);
		free(save);
		save = next;
	}
}

int	main(int argc, char **argv, char **env)
{
	char	*input;

	if (argc != 1)
	{
		printf("Error input\n");
		exit(1);
	}
	input = "1";
	while (input)
	{
		input = readline("Minishell> ");
		if (input)
		{
			add_history(input);
			argv = ft_split(input, ' ');
			if (argv)
			{
				ft_heredoc(argv, env);
				ft_free_tab(argv);
				argv = NULL;
			}
			free(input);
		}
	}
}
