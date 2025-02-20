/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/20 13:58:32 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_absolut_path(t_data *data, t_cmd *cur)
{
	char	*path;
	char	*target_path;

	path = NULL;
	if (ft_strlen(data->pwd) == ft_strlen(cur->args[1])
		&& !ft_strncmp(data->pwd, cur->args[1], ft_strlen(data->pwd)))
		return ;
	target_path = ft_strdup(ft_strsstr(data->pwd, cur->args[1]));
	if (!target_path)
	{
		path = ft_strjoin(data->pwd, "/");
		target_path = ft_strjoin(path, cur->args[1]);
		free(path);
	}
	if (chdir(target_path))
	{
		errors_exec(cur->args[1], "cd", FILES);
		data->gexit_code = 1;
	}
	free(target_path);
}

void	ft_update_extension(t_data *data, t_lst *tmp, char *var)
{
	char	*pwd;

	pwd = ft_find_pwd(data);
	var = ft_strjoin(var, "=");
	if (pwd)
	{
		free(tmp->str);
		tmp->str = ft_strjoin(var, pwd);
		free(pwd);
	}
	free(var);
}

void	ft_update_pwd(t_data *data, int info)
{
	t_lst	*tmp;
	char	*var;
	char	*get_pwd;
	char	*get_oldpwd;
	char	*res;
	char	*pwd;
	char	*var_value;
	char	*value;

	res = NULL;
	tmp = data->env;
	var = NULL;
	var_value = ft_get_var_and_value("PWD", tmp);
	get_pwd = my_getenv_lst("PWD", tmp);
	get_oldpwd = my_getenv_lst("OLDPWD", tmp);
	pwd = ft_find_pwd(data);
	value = pwd;
	if (var_value)
		value = ft_get_value(var_value);
	if (!get_pwd)
		ft_unset_extension("OLDPWD", tmp);
	else if (get_pwd)
	{
		ft_export_assign(data, "PWD", pwd);
		if (!get_oldpwd)
			ft_export_assign(data, "OLDPWD", data->pwd);
	}
	if (var_value && pwd != value)
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
				res = ft_strjoin(var, "=");
				free(tmp->str);
				tmp->str = ft_strjoin(res, value);
				free(res);
			}
			free(var);
			tmp = tmp->next;
		}
	}
	if (var_value)
		free(value);
	free(pwd);
	free(get_pwd);
	free(get_oldpwd);
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
	if (!cur->args[1] || cur->args[1][0] == '\0')
	{
		ft_putendl_fd("cd : expected a relative or absolute path", 2);
		data->gexit_code = 1;
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
