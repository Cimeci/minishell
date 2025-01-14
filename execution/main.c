/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:36:35 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/14 15:01:17 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_apply_funct(char **argv, t_env *list, char *input)
{
	int	info;

	info = ft_env(argv, list);
	if (info)
		return (1);
	else if (info == -1)
		return (-1);
	info = ft_unset(argv, list);
	if (info)
		return (1);
	else if (info == -1)
		return (-1);
	info = ft_export(argv, list);
	if (info)
		return (1);
	else if (info == -1)
		return (-1);
	info = ft_cd(argv);
	if (info)
		return (1);
	else if (info == -1)
		return (-1);
	info = ft_echo(ft_words(input, ' '), argv);
	if (info)
		return (1);
	info = ft_pwd(argv);
	if (info)
		return (1);
	else if (info == -1)
		return (-1);
	ft_exit(argv);
	return (0);
}

char	*use_input(char **argv, t_env *list)
{
	char	*input;
	t_env	*tmp;

	input = readline("Minishell> ");
	if (input && input[0])
	{
		add_history(input);
		argv = ft_split(input, ' ');
		if (argv)
		{
			tmp = list;
			if (!ft_apply_funct(argv, tmp, input))
				execution_cmd(argv, ft_convert_lst_to_tab(tmp));
			ft_free_tab(argv);
			argv = NULL;
		}
	}
	return (input);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_env	*list;

	if (argc != 1)
	{
		printf("Error input\n");
		exit(1);
	}
	input = "1";
	list = ft_init_env(env);
	while (input)
	{
		input = use_input(argv, list);
		if (input)
			free(input);
	}
	ft_lstclear2(list);
}
