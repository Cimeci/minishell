/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 15:05:11 by inowak--         ###   ########.fr       */
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

int	ft_unset(char **argv, t_env *env)
{
	char	*unset_path;
	int		i;

	i = 1;
	if (!ft_strncmp(argv[0], "unset", ft_strlen(argv[0])))
	{
		while (argv[i])
		{
			unset_path = get_path(argv[i], env);
			if (!unset_path)
				return (0);
			ft_unset_extension(unset_path, env);
			i++;
		}
		return (1);
	}
	return (0);
}