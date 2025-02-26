/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:28:32 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/26 18:10:29 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_env(t_data *data, char **env)
{
	t_lst	*cur;
	int		i;

	cur = data->env;
	i = 0;
	while (env[i])
	{
		cur = (t_lst *)ft_lstnew_generic(sizeof(t_lst));
		if (!cur)
			return ;
		cur->str = ft_strdup(env[i]);
		cur->next = NULL;
		ft_lstadd_back_generic((void **)&data->env, cur, sizeof(char *));
		i++;
	}
}

void	init_data(t_data *data, char **env)
{
	int		i;
	char	buffer[BUFFER_SIZE];
	char	*check;

	i = 0;
	check = getcwd(buffer, BUFFER_SIZE);
	if (check)
		data->pwd = NULL;
	else
		perror("Error getcwd");
	get_shell_pid(data);
	data->line = NULL;
	data->token = NULL;
	data->cmd = NULL;
	data->env = NULL;
	data->env_cp = NULL;
	data->fd[0] = -1;
	data->fd[1] = -1;
	data->gexit_code = 0;
	data->env = NULL;
	data->pid = NULL;
	init_env(data, env);
}
