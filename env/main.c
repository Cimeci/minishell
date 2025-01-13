/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 10:52:03 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	if (table)
	{
		while (table[i])
		{
			if (table[i])
				free(table[i]);
			i++;
		}
		free(table);
		table = NULL;
	}
}

t_env	*ft_init_env(char **env)
{
	t_env	*s_env;
	t_env	*list;
	int		i;

	i = 0;
	list = NULL;
	while (env[i])
	{
		s_env = malloc(sizeof(t_env));
		s_env->path = ft_strdup(env[i++]);
		s_env->next = NULL;
		ft_lstadd_back2(&list, s_env);
	}
	return (list);
}

t_env	*ft_env(char **argv, t_env *env)
{
	t_env	*cur;

	cur = env;
	if (!ft_strncmp(argv[0], "env", ft_strlen(argv[0])))
	{
		while (cur)
		{
			printf("%s\n", cur->path);
			cur = cur->next;
		}
	}
	else
		execution_cmd(argv, ft_convert_lst_to_tab(cur));
	return (env);
}

char	*use_input(char *input, t_env *list, char **argv)
{
	input = readline("Minishell> ");
	if (input && input[0])
	{
		add_history(input);
		argv = ft_split(input, ' ');
		if (argv)
		{
			list = ft_env(argv, list);
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
		input = use_input(input, list, argv);
		if (input)
			free(input);
	}
	rl_clear_history();
	ft_lstclear2(list);
}
