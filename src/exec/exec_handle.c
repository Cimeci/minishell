/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:02:20 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/26 11:52:09 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_command_execution(t_data *data, t_cmd *cur)
{
	DIR	*directory;

	if ((!ft_strncmp(cur->cmd, ".", 1) && ft_strlen(cur->cmd) == 1)
		|| (!ft_strncmp(cur->cmd, "..", 2) && ft_strlen(cur->cmd) == 2))
	{
		errors(data, cur->args[0], CMD_NOT_FOUND);
		if (!ft_strncmp(cur->cmd, ".", 1) && ft_strlen(cur->cmd) == 1)
		{
			free_all(data, 0);
			exit(2);
		}
		free_all(data, 0);
		exit(127);
	}
	directory = opendir(cur->cmd);
	if (directory != NULL)
	{
		errors(data, cur->args[0], DIRECTORY);
		closedir(directory);
		free_all(data, 0);
		exit(126);
	}
}

static void	check_command_access(t_data *data, t_cmd *cur, char *get_env)
{
	char	*var;
	t_lst	*tmp;
	
	var = NULL;	
	tmp = data->env;
	while (tmp)
	{
		if (tmp->str && ft_strcmp(tmp->str, "PATH="))
		{
			var = ft_get_value(tmp->str);
			break; 
		}
		tmp = tmp->next;
	}
	if (var && var[0] == '\0' && !access(cur->cmd, X_OK))
		return ;
	if (get_env && !ft_strnstr(cur->cmd, "/", ft_strlen(cur->cmd)))
	{
		free(get_env);
		errors(data, cur->args[0], CMD_NOT_FOUND);
		free_all(data, 0);
		exit(127);
	}
	free(get_env);
	if (access(cur->cmd, F_OK) == 0)
	{
		if (access(cur->cmd, X_OK) != 0)
		{
			errors(data, cur->args[0], PERM);
			free_all(data, 0);
			exit(126);
		}
	}
	if (access(cur->cmd, X_OK))
	{
		errors(data, cur->args[0], CMD_NOT_FOUND);
		free_all(data, 0);
		exit(127);
	}
}

void	handle_command_execution(t_data *data, t_cmd *cur)
{
	char	*get_env;

	get_env = NULL;
	rl_clear_history();
	if (!cur->cmd)
	{
		free_all(data, 0);
		return ;
	}
	if (!exec_built_in(data, cur))
	{
		check_command_execution(data, cur);
		get_env = my_getenv_lst("PATH", data->env);
		check_command_access(data, cur, get_env);
		execve(cur->cmd, cur->args, data->env_cp);
	}
	free_all(data, 0);
}
