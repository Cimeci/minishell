/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/17 10:08:26 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_absolut_path(t_data *data, t_cmd *cur)
{
	char	*path;
	char	*target_path;

	target_path = ft_strsstr(data->pwd, cur->args[1]);
	if (!target_path)
	{
		path = ft_strjoin(data->pwd, "/");
		target_path = ft_strjoin(path, cur->args[1]);
	}
	if (chdir(target_path))
	{
		errors_exec(cur->args[1], "cd", FILES);
		data->gexit_code = 1;
	}
}

void	ft_update_extension(t_data *data, t_lst *tmp, char *var)
{
	var = ft_strjoin(var, "=");
	tmp->str = ft_strjoin(var, ft_find_pwd(data));
}

void	ft_update_pwd(t_data *data, int info)
{
	t_lst	*tmp;
	char	*var;

	tmp = data->env;
	if (!my_getenv_lst("PWD", tmp))
		ft_unset_extension("OLDPWD", tmp);
	if (ft_find_pwd(data) != ft_get_value(ft_get_var_and_value("PWD", tmp)))
	{
		while (tmp)
		{
			var = ft_get_var(tmp->str);
			if (!ft_strncmp(var, "PWD", ft_strlen(var)) && ft_strlen(var) == 3
				&& info == 1)
				ft_update_extension(data, tmp, var);
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
}

int	ft_cd_root(t_data *data, t_cmd *cur)
{
	if (!ft_strncmp(cur->args[1], "/", ft_strlen(cur->args[1])))
	{
		chdir("/");
		data->pwd = ft_strdup("/");
		ft_update_pwd(data, 1);
		return (0);
	}
	return (1);
}

int	ft_cd(t_data *data, t_cmd *cur)
{
	//char	*root;

	//root = NULL;
	if (!cur->args[1] || cur->args[1][0] == '\0')
	{
		ft_putendl_fd("cd : No path", 2);
		data->gexit_code = 2;
		return (2);
	}
	if (ft_strlen_tab(cur->args) != 2)
	{
		errors(data, "cd", ARGS);
		data->gexit_code = 1;
		return (2);
	}
	if (cur->args[1] && cur->args[1][0] != '\0')
	{
		ft_update_pwd(data, 0);
		if (!ft_cd_root(data, cur))
			return (1);
		ft_absolut_path(data, cur);
		ft_update_pwd(data, 1);
		return (1);
	}
	return (0);
}
