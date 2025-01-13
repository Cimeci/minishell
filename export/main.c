/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 11:27:46 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_var(char *str)
{
	char	*var;
	int		i;

	i = 0;
	while (str[i] != '=' && str[i])
		i++;
	var = ft_substr(str, 0, i);
	return (var);
}

void	ft_modif_env_var(t_env *cur, t_env *env, char *var, char *arg)
{
	char	*path;

	while (cur)
	{
		path = my_getenv_lst(cur->path, env);
		if (!ft_strncmp(var, path, ft_strlen(var)))
		{
			free(cur->path);
			cur->path = ft_strdup(arg);
			break ;
		}
		free(path);
		cur = cur->next;
	}
}

t_env	*ft_export(char **argv, t_env *env)
{
	char	*path;
	char	*var;
	t_env	*cur;
	int		i;

	i = 1;
	cur = env;
	if (!ft_strncmp(argv[0], "export", ft_strlen(argv[0])))
	{
		while (argv[i])
		{
			var = ft_get_var(argv[i]);
			path = my_getenv_lst(argv[i], env);
			if (!path)
				ft_lstadd_back2(&env, ft_lstnew2(ft_strdup(argv[i])));
			else
				ft_modif_env_var(cur, env, var, argv[i]);
			free(path);
			free(var);
			i++;
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
			list = ft_export(argv, list);
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
