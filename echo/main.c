/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/07 12:54:05 by inowak--         ###   ########.fr       */
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

void	ft_echo(char **argv, int argc)
{
	int	i;
	int	endl;
	int	fd;
	int	j;
	int	save;

	endl = 0;
	i = 1;
	fd = 0;
	// for (int j = 0; argv[j]; j++)
	// 	printf("|%s|\n", argv[j]);
	if (ft_strncmp("echo", argv[0], ft_strlen(argv[0])))
	{
		printf("Error input\n");
		return ;
	}
	if (!ft_strncmp("-n", argv[1], ft_strlen(argv[1])))
	{
		i++;
		endl--;
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
	int l = 0;
	j = save;
	while (j < argc && argv[j])
	{
		if (!ft_strncmp(">", argv[j], ft_strlen(argv[j])) || !ft_strncmp(">>",
				argv[j], ft_strlen(argv[j])))
		{
			j += 2;
			l += 2;
			if (j >= argc || !argv[j])
				break ;
			continue ;
		}
		j++;
	}
	j = save;
	while (j < argc && argv[j])
	{
		// printf("argc:%d |dif:%d | j:%d\n", argc, argc - 1, j);
		// printf("bef:argv[%d] = %s\n", j, argv[j]);
		if (!ft_strncmp(">", argv[j], ft_strlen(argv[j])) || !ft_strncmp(">>",
			argv[j], ft_strlen(argv[j])))
		{
			j += 2;
			if (j > argc || !argv[j])
				break ;
		}
		// printf("aft:argv[%d] = %s\n", j, argv[j]);
		if (j < argc - l + 1)
		{
			ft_putstr_fd(argv[j], fd);
			ft_putstr_fd(" ", fd);
		}
		else if (endl < 0)
			ft_putstr_fd(argv[j], fd);
		else
			ft_putendl_fd(argv[j], fd);
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

int	main(int argc, char **argv)
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
			ft_echo(argv, ft_words(input, ' '));
			ft_free_tab(argv);
			argv = NULL;
			free(input);
		}
	}
}