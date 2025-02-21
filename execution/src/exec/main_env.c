/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/21 15:09:50 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_data *data, t_cmd *cur)
{
	t_lst	*tmp;
	char	*path;

	path = my_getenv_lst("PATH", data->env);
	if (!path)
	{
		errors(data, "env", FILES);
		free_all(data, 0);
		exit(127);
	}
	free(path);
	if (ft_strlen_tab(cur->args) != 1)
	{
		errors(data, "env", ARGS);
		free_all(data, 0);
		exit(1);
	}
	tmp = data->env;
	while (tmp)
	{
		printf("%s\n", tmp->str);
		tmp = tmp->next;
	}
	return (1);
}
