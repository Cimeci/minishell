/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_utils_echo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 10:27:29 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/23 09:54:04 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	write_no_arguments(char **argv, int endl, int save)
{
	int	j;

	j = save;
	if (endl >= 0)
	{
		if (((!ft_strncmp(argv[j], ">", ft_strlen(argv[j])))
				|| (!ft_strncmp(argv[j], ">>", ft_strlen(argv[j]))))
			&& (!ft_strncmp("echo", argv[j - 1], ft_strlen(argv[j - 1]))))
			printf("\n");
	}
}

int	ft_write_argv_space(char **argv, int j, int dif)
{
	if (j < dif - 1)
	{
		printf("%s", argv[j]);
		printf(" ");
		return (1);
	}
	return (0);
}

void	ft_write_argv_endl(char **argv, int j, int endl)
{
	if (endl < 0)
		printf("%s", argv[j]);
	else
		printf("%s\n", argv[j]);
}

void	write_arguments(char **argv, int endl, int save)
{
	int	j;
	int	l;
	int	argc;

	argc = 0;
	while (argv[argc])
		argc++;
	l = count_trailing_redirects(argv, argc);
	j = save;
	while (j < argc && argv[j])
	{
		if ((ft_strncmp(argv[j], ">", ft_strlen(argv[j]))
				&& ft_strncmp(argv[j - 1], ">", ft_strlen(argv[j])))
			&& (ft_strncmp(argv[j], ">>", ft_strlen(argv[j]))
				&& ft_strncmp(argv[j - 1], ">>", ft_strlen(argv[j - 1]))))
		{
			if (!ft_write_argv_space(argv, j, argc - l))
				ft_write_argv_endl(argv, j, endl);
		}
		j++;
	}
}

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
