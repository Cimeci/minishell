/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/14 18:21:31 by inowak--         ###   ########.fr       */
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

void	ft_print_env_export(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		printf("declare -x %s\n", cur->path);
		cur = cur->next;
	}
}

int	ft_check_env_var(char *var)
{
	int	i;

	i = 1;
	if (!ft_isalpha(var[0]))
	{
		if (var[0] != '_')
			return (0);
	}
	while (var[i])
	{
		if (!ft_isalnum(var[i]))
		{
			if (var[i] != '_')
				return (0);
		}
		i++;
	}
	return (1);
}

int	ft_export(char **argv, t_env *env)
{
	char	*path;
	char	*var;
	char	*var_check;
	t_env	*cur;
	int		i;

	i = 1;
	cur = env;
	if (!ft_strncmp(argv[0], "export", ft_strlen(argv[0])))
	{
		if (!argv[1])
		{
			ft_print_env_export(cur);
			return (1);
		}
		while (argv[i])
		{
			var_check = ft_get_var(argv[i]);
			if (ft_check_env_var(var_check))
			{
				var = ft_get_var(argv[i]);
				path = my_getenv_lst(var, env);
				if (!path)
					ft_lstadd_back2(&env, ft_lstnew2(ft_strdup(argv[i])));
				else
					ft_modif_env_var(cur, env, var, argv[i]);
				free(path);
				free(var);
			}
			free(var_check);
			i++;
		}
		return (1);
	}
	return (0);
}
