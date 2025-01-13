/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 14:48:27 by inowak--         ###   ########.fr       */
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

int	ft_export(char **argv, t_env *env)
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
		return (1);
	}
	return (0);
}
