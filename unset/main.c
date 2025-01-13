/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 11:46:28 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(const char *name, t_env *env)
{
	int		j;
	char	*tmp;

	while (env)
	{
		j = 0;
		while (env->path[j] != '=' && env->path[j])
			j++;
		tmp = ft_substr(env->path, 0, j);
		if (ft_strncmp(tmp, name, j) == 0 && name[j] == '\0')
		{
			free(tmp);
			return (env->path);
		}
		free(tmp);
		env = env->next;
	}
	return (NULL);
}

void	ft_unset_extension(char *unset_path, t_env *env)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = env;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strncmp(tmp->path, unset_path, ft_strlen(tmp->path)))
		{
			if (prev == NULL)
				env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->path);
			free(tmp);
			tmp = NULL;
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	ft_unset(char **argv, t_env *env)
{
	char	*unset_path;
	int		i;
	char	**env_tab;

	i = 1;
	if (!ft_strncmp(argv[0], "unset", ft_strlen(argv[0])))
	{
		while (argv[i])
		{
			unset_path = get_path(argv[i], env);
			if (!unset_path)
				return ;
			ft_unset_extension(unset_path, env);
			i++;
		}
	}
	else
	{
		env_tab = ft_convert_lst_to_tab(env);
		execution_cmd(argv, env_tab);
		ft_free_tab(env_tab);
	}
}

char	*use_input(char **argv, t_env *list)
{
	char	*input;

	input = readline("Minishell> ");
	if (input && input[0])
	{
		add_history(input);
		argv = ft_split(input, ' ');
		if (argv)
		{
			ft_unset(argv, list);
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
