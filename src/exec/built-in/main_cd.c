/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/25 10:01:03 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error_cd(t_data *data, t_cmd *cur, DIR *check_dir)
{
	char	*error;

	error = ft_strjoin("cd: ", cur->args[1]);
	if (check_dir == NULL)
	{
		errors(data, error, PERM);
		closedir(check_dir);
	}
	else
		errors_exec(cur->args[1], "cd", FILES);
	free(error);
	data->gexit_code = 1;
}

void	ft_absolut_path(t_data *data, t_cmd *cur)
{
	char	*path;
	char	*target_path;
	DIR		*check_dir;

	path = NULL;
	target_path = NULL;
	check_dir = NULL;
	if (ft_strlen(data->pwd) == ft_strlen(cur->args[1])
		&& !ft_strncmp(data->pwd, cur->args[1], ft_strlen(data->pwd)))
		return ;
	check_dir = opendir(cur->args[1]);
	target_path = ft_strdup(ft_strsstr(data->pwd, cur->args[1]));
	if (!target_path)
	{
		path = ft_strjoin(data->pwd, "/");
		target_path = ft_strjoin(path, cur->args[1]);
		free(path);
	}
	if (chdir(target_path))
		ft_error_cd(data, cur, check_dir);
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
		if (data->env)
			ft_update_pwd(data, 0);
		if (!ft_cd_root(data, cur))
			return (1);
		ft_absolut_path(data, cur);
		ft_update_pwd(data, 1);
		return (1);
	}
	return (0);
}
