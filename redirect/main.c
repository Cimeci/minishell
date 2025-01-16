/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/16 19:09:56 by inowak--         ###   ########.fr       */
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
		{
			dprintf(2, "ERROR\n");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
			execve(path, argv, env);
		else if (wait(NULL) == -1)
			perror("wait failed");
	}
	// ft_free_tab(env);
	free(path);
}

int	ft_strlen_tab(char **table)
{
	int	i;

	i = 0;
	while (table[i])
		i++;
	return (i);
}

void	ft_print_tab(char **table)
{
	int	i;

	i = 0;
	dprintf(2, "%d\n", ft_strlen_tab(table));
	while (table[i])
		dprintf(2, "pt: %s\n", table[i++]);
}

int	count_trailing_redirects(char **argv, int argc)
{
	int	j;
	int	l;

	l = 0;
	j = argc - 1;
	while (j > 0)
	{
		if (!ft_strncmp(argv[j], ">", ft_strlen(argv[j])) || !ft_strncmp(argv[j
				- 1], ">", ft_strlen(argv[j])) || !ft_strncmp(argv[j], ">>",
				ft_strlen(argv[j])) || !ft_strncmp(argv[j - 1], ">>",
				ft_strlen(argv[j])))
			l++;
		else
			break ;
		j--;
	}
	return (l);
}

char	**find_args(char **argv)
{
	int		i;
	int		j;
	int		k;
	int		argc;
	char	**args;

	argc = ft_strlen_tab(argv);
	k = count_trailing_redirects(argv, argc);
	i = 0;
	// dprintf(2, "%d\n", argc - k + 1);
	args = malloc(sizeof(char *) * (argc - k + 1));
	if (!args)
		return (NULL);
	j = 0;
	if (ft_strncmp(argv[0], ">", ft_strlen(argv[0])) && ft_strncmp(argv[0],
			">>", ft_strlen(argv[0])))
	{
		args[j] = ft_strdup(argv[0]);
		j++;
	}
	i = 1;
	while (argv[i])
	{
		// dprintf(2, "%s\n", argv[i]);
		if ((ft_strncmp(argv[i], ">", ft_strlen(argv[i])) && ft_strncmp(argv[i
					- 1], ">", ft_strlen(argv[i]))) && (ft_strncmp(argv[i],
					">>", ft_strlen(argv[i])) && ft_strncmp(argv[i - 1], ">>",
					ft_strlen(argv[i - 1]))))
		{
			args[j] = ft_strdup(argv[i]);
			j++;
		}
		i++;
	}
	args[j] = NULL;
	// ft_print_tab(args);
	return (args);
}

static int	open_output_file(char **argv)
{
	int	fd;
	int	i;

	fd = 0;
	i = 0;
	while (argv[i])
	{
		if (!ft_strncmp(">", argv[i], ft_strlen(argv[i])))
			fd = open(argv[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0664);
		else if (!ft_strncmp(">>", argv[i], ft_strlen(argv[i])))
			fd = open(argv[i + 1], O_WRONLY | O_APPEND | O_CREAT, 0664);
		if (fd < 0)
			return (-1);
		i++;
	}
	return (fd);
}

void	ft_redirect(char **argv, char **env)
{
	int		fd;
	pid_t	pid;
	char	*path;
	char	**args;

	fd = open_output_file(argv);
	if (fd <= 0)
	{
		execution_cmd(argv, env);
		return ;
	}
	args = find_args(argv);
	if (!args)
	{
		dprintf(2, "ERROR\n");
		return ;
	}
	path = find_path(env, args[0]);
	if (!path)
	{
		dprintf(2, "Command not found: %s\n", argv[0]);
		close(fd);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		dprintf(2, "ERROR\n");
		free(path);
		close(fd);
		return ;
	}
	else if (pid == 0)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			dprintf(2, "ERROR\n");
			return ;
		}
		close(fd);
		execve(path, args, env);
		dprintf(2, "ERROR\n");
		return ;
	}
	else
	{
		close(fd);
		while (wait(NULL) > 0)
			;
	}
	free(path);
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
				ft_redirect(argv, env);
				ft_free_tab(argv);
				argv = NULL;
			}
			free(input);
		}
	}
}
