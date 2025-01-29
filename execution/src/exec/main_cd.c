/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/29 10:03:15 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_absolut_path(t_data *data, t_cmd *cur)
{
	char		*path;
	char		*target_path;

	target_path = ft_strsstr(data->pwd, cur->args[1]);
	if (!target_path)
	{
		path = ft_strjoin(data->pwd, "/");
		target_path = ft_strjoin(path, cur->args[1]);
	}
	if (chdir(target_path))
	{
		// ft_free_path(target_path, path);
		error_exec(data, cur->args[1], "cd", 3);
	}
}

void	ft_update_pwd(t_data *data, int info)
{
	t_lst	*tmp;
	char	*var;

	tmp = data->env;
	if (!my_getenv_lst("PWD", tmp))
		ft_unset_extension("OLDPWD", tmp);
	// else
	// 	ft_export("OLDPWD", tmp); pensee a actualliser OLD_PWD
	while (tmp)
	{
		var = ft_get_var(tmp->str);
		if (!ft_strncmp(var, "PWD", ft_strlen(var)) && ft_strlen(var) == 3
			&& info == 1)
		{
			var = ft_strjoin(var, "=");
			tmp->str = ft_strjoin(var, ft_find_pwd());
		}
		else if (!ft_strncmp(var, "OLDPWD", ft_strlen(var))
			&& ft_strlen(var) == 6 && info == 0)
		{
			var = ft_strjoin(var, "=");
			tmp->str = ft_strjoin(var, data->pwd);
		}
		free(var);
		tmp = tmp->next;
	}
}

int	ft_cd(t_data *data, t_cmd *cur)
{
	char	*root;

	root = NULL;
	if (!cur->args[1] || cur->args[1][0] == '\0')
	{
		ft_putendl_fd("cd : No path", 2);
		return (-1);
	}
	if (ft_strlen_tab(cur->args) != 2)
		return (error_exec(data, NULL, "cd", 4));
	if (cur->args[1] && cur->args[1][0] != '\0')
	{
		ft_update_pwd(data, 0);
		if (!ft_strncmp(cur->args[1], "/", ft_strlen(cur->args[1])))
		{
			data->pwd = ft_strdup("/");
			if (chdir("/"))
			{
				perror("Error");
				return (-1);
			}
			ft_update_pwd(data, 1);
			return (1);
		}
		ft_absolut_path(data, cur);
		ft_update_pwd(data, 1);
		return (1);
	}
	return (0);
}
