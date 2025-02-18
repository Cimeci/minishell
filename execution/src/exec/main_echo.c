/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/18 18:30:55 by inowak--         ###   ########.fr       */
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

void	write_arguments(char **argv, int endl, int save)
{
	int	j;
	int	argc;

	argc = ft_strlen_tab(argv);
	j = save;
	if (!argv[j])
		return ;
	while (j < argc - 1)
	{
		if (argv[j] && argv[j][0] != '\0')
		{
			printf("%s", argv[j]);
			printf(" ");
		}
		j++;
	}
	if (endl < 0)
		printf("%s", argv[j]);
	else
		printf("%s\n", argv[j]);
}

int	ft_echo(char **argv)
{
	int	endl;
	int	save;

	if ((!argv[1]))
	{
		printf("\n");
		return (1);
	}
	endl = 0;
	save = handle_options(argv, &endl);
	write_arguments(argv, endl, save);
	return (1);
}
