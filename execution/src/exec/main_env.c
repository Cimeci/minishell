/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/21 17:53:20 by inowak--         ###   ########.fr       */
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

	if (!ft_strncmp(cur->cmd, "env", ft_strlen(cur->cmd)))
		return (-1);
	tmp = data->env;
	while (tmp)
	{
		printf("%s\n", tmp->str);
		tmp = tmp->next;
	}
	return (1);
}
