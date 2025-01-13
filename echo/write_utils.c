/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 10:27:29 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 10:28:16 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_no_arguments(char **argv, int endl, int save, int fd)
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

void	ft_write_argv_space(char **argv, int j, int dif, int fd)
{
	if (j < dif - 1)
	{
		ft_putstr_fd(argv[j], fd);
		ft_putstr_fd(" ", fd);
	}
}

void	ft_write_argv_endl(char **argv, int j, int endl, int fd)
{
	if (endl < 0)
		ft_putstr_fd(argv[j], fd);
	else
		ft_putendl_fd(argv[j], fd);
}

void	write_arguments(char **argv, int fd, int endl, int save)
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
			ft_write_argv_space(argv, j, argc - l, fd);
			ft_write_argv_endl(argv, j, endl, fd);
		}
		j++;
	}
}
