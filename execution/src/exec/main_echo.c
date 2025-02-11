/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/11 16:27:58 by inowak--         ###   ########.fr       */
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
		if (argv[i][w] == '-' && argv[i][w + 1])
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

int	ft_echo(char **argv)
{
	int	endl;
	int	save;

	if (!argv[1])
	{
		printf("\n");
		return (1);
	}
	endl = 0;
	save = handle_options(argv, &endl);
	write_no_arguments(argv, endl, save);
	write_arguments(argv, endl, save);
	return (1);
}
