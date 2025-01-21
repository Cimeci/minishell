/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 10:49:30 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/21 18:14:03 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*my_getenv(t_data *data, char *name)
{
	t_lst	*cur;
	char	*tmp;
	int		i;

	cur = data->env;
	i = 0;
	while (cur)
	{
		i = 0;
		while (cur->str[i] && cur->str[i] != '=')
			i++;
		tmp = ft_substr(cur->str, 0, i);
		if (ft_strncmp(tmp, name, i) == 0 && name[i] == '\0')
		{
			free(tmp);
			return (cur->str + i + 1);
		}
		cur = cur->next;
		free(tmp);
	}
	return (NULL);
}

char	*find_path(t_data *data, char *str)
{
	char	*path;
	char	**path_split;
	char	*pathname;
	char	*tmp;
	int		i;

	i = 0;
	path = my_getenv(data, "PATH");
	if (!path)
		return (NULL);
	path_split = ft_split(path, ':');
	while (path_split[i])
	{
		tmp = ft_strjoin(path_split[i], "/");
		pathname = ft_strjoin(tmp, str);
		free(tmp);
		if (!access(pathname, X_OK))
		{
			ft_free_tab(path_split);
			return (pathname);
		}
		free(pathname);
		i++;
	}
	ft_free_tab(path_split);
	return (NULL);
}

// void	execution_cmd(t_data *data)
// {
// 	char	*path;
// 	pid_t	pid;

// 	if (!data->env)
// 		return ;
// 	if (!data->cmd->cmd)
// 		return ;
// 	if (data->cmd->cmd && access(data->cmd->cmd, X_OK) == 0)
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 			exit(EXIT_FAILURE);
// 		else if (pid == 0)
// 			execve(data->cmd->cmd, data->cmd->args, data->env);
// 		else if (wait(NULL) == -1)
// 			perror("wait failed");
// 	}
// 	ft_free_tab(data->env);
// 	free(data->cmd->cmd);
// }
