/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 17:49:28 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/20 18:06:33 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_update_pwd(t_data *data, int info)
{
	t_lst	*tmp;
	char	*pwd;
	char	*get_pwd;
	char	*get_oldpwd;

	tmp = data->env;
	pwd = ft_find_pwd(data);
	get_pwd = my_getenv_lst("PWD", tmp);
	get_oldpwd = my_getenv_lst("OLDPWD", tmp);
	ft_handle_pwd_update(data, tmp, pwd, get_pwd);
	ft_handle_oldpwd_update(data, get_oldpwd, get_pwd);
	if (info == 1)
		ft_update_env_vars(tmp, pwd, "PWD");
	else if (info == 0)
		ft_update_env_vars(tmp, pwd, "OLDPWD");
	free(pwd);
	free(get_pwd);
	free(get_oldpwd);
}

void	ft_handle_pwd_update(t_data *data, t_lst *tmp, char *pwd, char *get_pwd)
{
	if (!get_pwd)
		ft_unset_extension("OLDPWD", tmp);
	else
		ft_export_assign(data, "PWD", pwd);
}

void	ft_handle_oldpwd_update(t_data *data, char *get_oldpwd, char *get_pwd)
{
	if (!get_oldpwd && get_pwd)
		ft_export_assign(data, "OLDPWD", data->pwd);
}

void	ft_update_env_vars(t_lst *tmp, char *pwd, char *var_name)
{
	char	*var;
	char	*res;
	char	*var_value;

	while (tmp)
	{
		var = ft_get_var(tmp->str);
		if (!ft_strncmp(var, var_name, ft_strlen(var)))
		{
			if (ft_strlen(var) == ft_strlen(var_name))
			{
				var_value = ft_get_value(ft_get_var_and_value(var_name, tmp));
				if (pwd != var_value)
				{
					res = ft_strjoin(var, "=");
					free(tmp->str);
					tmp->str = ft_strjoin(res, pwd);
					free(res);
				}
				free(var_value);
			}
		}
		free(var);
		tmp = tmp->next;
	}
}
