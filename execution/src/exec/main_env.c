/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/29 15:33:20 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	ft_env(t_data *data, t_cmd *cur)
{
	t_lst	*tmp;

	if (!my_getenv_lst("PATH", data->env))
	{
		errors(data, "env", FILES);
		exit (127);
	}
	if (ft_strlen_tab(cur->args) != 1)
	{
		errors(data, "env", ARGS);
		exit (1);
	}
	tmp = data->env;
	while (tmp)
	{
		printf("%s\n", tmp->str);
		tmp = tmp->next;
	}
	return (1);
}
