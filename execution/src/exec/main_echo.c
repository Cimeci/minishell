/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/22 14:07:59 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_options(char **argv, int *endl)
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

int	open_output_file(char **argv, int argc)
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

int	ft_echo(int argc, char **argv)
{
	int	endl;
	int	fd;
	int	save;

	if (ft_strncmp("echo", argv[0], ft_strlen(argv[0])))
		return (0);
	if (!argv[1])
	{
		ft_putendl_fd("", 1);
		return (1);
	}
	endl = 0;
	save = handle_options(argv, &endl);
	fd = open_output_file(argv, argc);
	if (fd < 0)
		return (0);
	write_no_arguments(argv, endl, save, fd);
	write_arguments(argv, fd, endl, save);
	if (fd)
		close(fd);
	return (1);
}
