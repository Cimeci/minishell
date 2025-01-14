/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/14 13:22:16 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_echo(int argc, char **argv, t_env *env)
{
	int	endl;
	int	fd;
	int	save;

	if (ft_strncmp("echo", argv[0], ft_strlen(argv[0])))
	{
		execution_cmd(argv, env);
		return ;
	}
	if (!find_path(ft_convert_lst_to_tab(env), argv[0]))
		return (-1);
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
	write_arguments(argv, fd, endl, save);
	if (fd)
		close(fd);
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
