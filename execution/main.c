/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:36:35 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/21 14:44:40 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_dup_node(t_env *env)
{
	t_env	*export_env;
	t_env	*cur;
	t_env	*s_export_env;

	cur = env;
	export_env = NULL;
	while (env)
	{
		s_export_env = malloc(sizeof(t_env));
		s_export_env->path = ft_strdup(env->path);
		s_export_env->next = NULL;
		ft_lstadd_back2(&export_env, s_export_env);
		env = env->next;
	}
	return (export_env);
}

int	ft_apply_funct(char **argv, t_env *list, char *input, t_env *export_env)
{
	int	info;

	info = ft_env(argv, list);
	if (info)
		return (1);
	else if (info == -1)
		return (-1);
	info = ft_unset(argv, list, export_env);
	if (info)
		return (1);
	else if (info == -1)
		return (-1);
	info = ft_export(argv, list, export_env);
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

char	*use_input(char **argv, t_env *list, t_env *export_env)
{
	char	*input;
	t_env	*tmp;
	t_env	*tmp_export;

	input = readline("Minishell> ");
	if (input && input[0])
	{
		add_history(input);
		argv = ft_split(input, ' ');
		if (argv)
		{
			tmp = list;
			tmp_export = export_env;
			if (!ft_apply_funct(argv, tmp, input, tmp_export))
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
	t_env	*export_env;

	if (argc != 1)
	{
		printf("Error input\n");
		exit(1);
	}
	input = "1";
	list = ft_init_env(env);
	export_env = ft_dup_node(list);
	while (input)
	{
		input = use_input(argv, list, export_env);
		if (input)
			free(input);
	}
	ft_lstclear2(list);
	ft_lstclear2(export_env);
}
