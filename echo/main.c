/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/09 11:05:00 by inowak--         ###   ########.fr       */
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

void	ft_echo(int argc, char **argv, char **env)
{
	int	i;
	int	endl;
	int	fd;
	int	j;
	int	k;
	int	w;
	int	save;
	int	l;

	endl = 0;
	i = 1;
	fd = 0;
	if (ft_strncmp("echo", argv[0], ft_strlen(argv[0])))
	{
		execution_cmd(argv, env);
		return ;
	}
	k = 1;
	while (argv[k])
	{
		w = 0;
		if (argv[k][w] == '-')
		{
			w++;
			while (argv[k][w] == 'n')
				w++;
			if (argv[k][w] == '\0')
			{
				endl--;
				i++;
			}
			else
				break ;
		}
		k++;
	}
	save = i;
	while (i < argc)
	{
		if (!ft_strncmp(">", argv[i], ft_strlen(argv[i])))
		{
			fd = open(argv[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0664);
			if (fd < 0)
				exit(1);
		}
		else if (!ft_strncmp(">>", argv[i], ft_strlen(argv[i])))
		{
			fd = open(argv[i + 1], O_WRONLY | O_APPEND | O_CREAT, 0664);
			if (fd < 0)
				exit(1);
		}
		i++;
	}
	j = save;
	if (endl >= 0)
	{
		if (((!ft_strncmp(argv[j], ">", ft_strlen(argv[j])))
				|| (!ft_strncmp(argv[j], ">>", ft_strlen(argv[j]))))
			&& (!ft_strncmp("echo", argv[j - 1], ft_strlen(argv[j - 1]))))
			ft_putendl_fd("", fd);
	}
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
	j = save;
	while (j < argc && argv[j])
	{
		if ((ft_strncmp(argv[j], ">", ft_strlen(argv[j])) && ft_strncmp(argv[j
					- 1], ">", ft_strlen(argv[j]))) && (ft_strncmp(argv[j],
					">>", ft_strlen(argv[j])) && ft_strncmp(argv[j - 1], ">>",
					ft_strlen(argv[j - 1]))))
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

char	**ft_argv(char *input)
{
	int		i;
	int		j;
	int		k;
	int		t;
	char	**argv;

	i = 0;
	j = 0;
	k = 0;
	t = 0;
	argv = malloc(sizeof(char *) * (ft_words(input, ' ') + 1));
	if (!argv)
		return (NULL);
	while (input[i])
	{
		k = 0;
		t = 0;
		while (input[i] == ' ' && input[i])
			i++;
		k = i;
		while (input[i] != ' ' && input[i])
		{
			i++;
			t++;
		}
		argv[j] = ft_substr(input, k, t);
		if (!argv[j])
			return (NULL);
		j++;
	}
	argv[j] = NULL;
	return (argv);
}

int	main(int argc, char **argv, char **env)
{
	char *input;

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
			argv = ft_argv(input);
			ft_echo(ft_words(input, ' '), argv, env);
			ft_free_tab(argv);
			argv = NULL;
			free(input);
		}
	}
}
