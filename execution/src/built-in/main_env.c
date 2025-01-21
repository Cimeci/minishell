/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/15 16:30:47 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*tab_to_lower(char *str)
{
	int		i;
	char	*result;

	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	i = 0;
	while (str[i])
	{
		result[i] = ft_tolower(str[i]);
		i++;
	}
	result[i] = '\0';
	return (result);
}

void	ft_sort_env(char **env, int size)
{
	int		i;
	int		j;
	char	*temp;
	char	*tab1;
	char	*tab2;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			tab1 = tab_to_lower(env[j]);
			tab2 = tab_to_lower(env[j + 1]);
			if (ft_strncmp(tab1, tab2, ft_strlen(env[j])) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
			free(tab1);
			free(tab2);
			j++;
		}
		i++;
	}
}

int	ft_env(char **argv, t_env *env)
{
	t_env	*cur;
	char	**table_env;
	int		i;
	char	*pathname;
	int		fd;

	fd = open_output_file(argv, ft_strlen_tab(argv));
	i = 0;
	cur = env;
	if (!ft_strncmp(argv[0], "env", ft_strlen(argv[0])))
	{
		table_env = ft_convert_lst_to_tab(env);
		pathname = find_path(table_env, argv[0]);
		if (!pathname)
		{
			ft_free_tab(table_env);
			return (-1);
		}
		free(pathname);
		// ft_sort_env(table_env, ft_strlen_tab(table_env));
		while (table_env[i])
			ft_putendl_fd(table_env[i++], fd);
		ft_free_tab(table_env);
		return (1);
	}
	return (0);
}
