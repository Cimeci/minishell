/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/09 13:56:16 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_file(char **argv)
{
	int	file;
	int	i;

	file = 0;
	i = 0;
	while (argv[i])
	{
		if (!ft_strncmp(">", argv[i], ft_strlen(argv[i])) || !ft_strncmp(">>",
				argv[i], ft_strlen(argv[i])))
			file++;
		i++;
	}
	return (file);
}

void	execution_cmd(char **argv, char **env)
{
	char	*path;
	pid_t	pid;

	path = ft_strjoin("/usr/bin/", argv[0]);
	if (!path)
		return ;
	if (path && access(path, X_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		else if (pid == 0)
		{
			execve(path, argv, env);
			perror("Error");
		}
		else
		{
			if (wait(NULL) == -1)
				perror("wait failed");
		}
	}
	free(path);
}

static int	handle_options(char **argv, int *endl)
{
	int	i;
	int	w;

	i = 1;
	while (argv[i])
	{
		w = 0;
		if (argv[i][w] == '-')
		{
			w++;
			while (argv[i][w] == 'n')
				w++;
			if (argv[i][w] == '\0')
			{
				(*endl)--;
				i++;
			}
			else
				break ;
		}
		else
			break ;
	}
	return (i);
}

static int	open_output_file(char **argv, int argc)
{
	int	fd;
	int	i;

	fd = 0;
	i = 1;
	while (i < argc)
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

static int	count_trailing_redirects(char **argv, int argc)
{
	int	j;
	int	l;

	l = 0;
	j = argc - 1;
	while (j > 0)
	{
		if (!ft_strncmp(argv[j], ">", ft_strlen(argv[j]))
			|| !ft_strncmp(argv[j - 1], ">", ft_strlen(argv[j]))
			|| !ft_strncmp(argv[j], ">>", ft_strlen(argv[j]))
			|| !ft_strncmp(argv[j - 1], ">>", ft_strlen(argv[j])))
			l++;
		else
			break ;
		j--;
	}
	return (l);
}

static void	write_no_arguments(char **argv, int endl, int save, int fd)
{
	int	j;

	j = save;
	if (endl >= 0)
	{
		if (((!ft_strncmp(argv[j], ">", ft_strlen(argv[j])))
				|| (!ft_strncmp(argv[j], ">>", ft_strlen(argv[j]))))
			&& (!ft_strncmp("echo", argv[j - 1], ft_strlen(argv[j - 1]))))
			ft_putendl_fd("", fd);
	}
}

static void	write_arguments(char **argv, int argc, int fd, int endl, int save)
{
	int	j;
	int	l;

	l = count_trailing_redirects(argv, argc);
	j = save;
	while (j < argc && argv[j])
	{
		if ((ft_strncmp(argv[j], ">", ft_strlen(argv[j]))
				&& ft_strncmp(argv[j - 1], ">", ft_strlen(argv[j])))
			&& (ft_strncmp(argv[j], ">>", ft_strlen(argv[j]))
				&& ft_strncmp(argv[j - 1], ">>", ft_strlen(argv[j - 1]))))
		{
			if (j < argc - l - 1)
			{
				ft_putstr_fd(argv[j], fd);
				ft_putstr_fd(" ", fd);
			}
			else if (endl < 0)
				ft_putstr_fd(argv[j], fd);
			else
				ft_putendl_fd(argv[j], fd);
		}
		j++;
	}
}

void	ft_echo(int argc, char **argv, char **env)
{
	int	endl;
	int	fd;
	int	save;

	if (ft_strncmp("echo", argv[0], ft_strlen(argv[0])))
	{
		execution_cmd(argv, env);
		return ;
	}
	if (!argv[1])
	{
		ft_putendl_fd("", 1);
		return ;
	}
	endl = 0;
	save = handle_options(argv, &endl);
	fd = open_output_file(argv, argc);
	if (fd < 0)
		return ;
	write_no_arguments(argv, endl, save, fd);
	write_arguments(argv, argc, fd, endl, save);
	if (fd)
		close(fd);
}

void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	while (table[i])
		free(table[i++]);
	free(table);
	table = NULL;
}

int	main(int argc, char **argv, char **env)
{
	char	*input;

	(void)argv;
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
			ft_echo(ft_words(input, ' '), argv, env);
			ft_free_tab(argv);
			argv = NULL;
			free(input);
		}
	}
}
