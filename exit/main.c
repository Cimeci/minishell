/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/07 16:16:07 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(char **argv)
{
	if (!strncmp(argv[0], "exit", ft_strlen(argv[0])))
		exit(0);
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

int	main(int argc, char **argv)
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
		if (input && input[0] != '\0')
		{
			add_history(input);
			argv = ft_split(input, ' ');
			if (argv)
			{
				ft_exit(argv);
				ft_free_tab(argv);
				argv = NULL;
			}
		}
		if (input)
			free(input);
	}
}
