/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/22 10:10:221 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_get_var(char *str)
{
	char	*var;
	int		i;

	i = 0;
	while (str[i] != '=' && str[i])
		i++;
	if (!str[i])
		return (NULL);
	var = ft_substr(str, 0, i);
	return (var);
}

void	ft_modif_env_var(t_lst *cur, t_lst *env, char *var, char *arg)
{
	char	*path;

	(void)env;
	while (cur)
	{
		path = ft_get_var(cur->str);
		// printf("%s|%s|%s\n", var, arg, path);
		if (path)
		{
			if (!ft_strncmp(var, path, ft_strlen(var)))
			{
				free(cur->str);
				free(path);
				cur->str = ft_strdup(arg);
				break ;
			}
		}
		free(path);
		path = NULL;
		cur = cur->next;
	}
}

void	ft_print_env_export(t_lst *env, char **argv)
{
	t_lst	*cur;
	int		fd;

	fd = open_output_file(argv, ft_strlen_tab(argv));
	cur = env;
	while (cur)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putendl_fd(cur->str, fd);
		cur = cur->next;
	}
}

int	ft_check_env_var(char *var)
{
	int	i;

	i = 1;
	if (!var)
		return (0);
	if (!ft_isalpha(var[0]))
	{
		if (var[0] != '_')
			return (0);
	}
	while (var[i])
	{
		if (!ft_isalnum(var[i]))
		{
			if (var[i] != '_')
				return (0);
		}
		i++;
	}
	return (1);
}

char	*ft_get_pvar(char *argv)
{
	int	i;

	i = 0;
	while (argv[i] != '+' && argv[i])
		i++;
	if (!argv[i])
		return (NULL);
	i++;
	if (argv[i] == '=')
		return (ft_substr(argv, 0, i - 1));
	return (NULL);
}

char	*ft_get_var_and_value(char *var, t_lst *env)
{
	int		j;
	char	*tmp;
	char	*tmp_dup;
	t_lst	*cur;

	cur = env;
	while (cur)
	{
		j = 0;
		while (cur->str[j] != '=' && cur->str[j])
			j++;
		tmp_dup = ft_strdup(cur->str);
		tmp = ft_substr(tmp_dup, 0, j);
		if (!ft_strncmp(tmp, var, ft_strlen(var)))
		{
			free(tmp_dup);
			free(tmp);
			return (cur->str);
		}
		free(tmp_dup);
		free(tmp);
		cur = cur->next;
	}
	return (NULL);
}

char	*ft_get_value(char *argv)
{
	int	i;

	i = 0;
	while (argv[i] != '=' && argv[i])
		i++;
	if (argv[i] == '=')
		return (ft_substr(argv, i + 1, ft_strlen(argv) - i));
	return (NULL);
}

int	ft_export(t_data *data, t_cmd *cur)
{
	char	*path;
	char	*var;
	char	*var_check;
	char	*p_var;
	char	*p_res;
	char	*value;
	t_lst	*tmp;
	t_lst	*tmp_export;
	t_lst	*node;
	t_lst	*node_export;
	int		i;

	i = 1;
	node = NULL;
	node_export = NULL;
	tmp = data->env;
	tmp_export = data->export_env;
	if (ft_strlen_tab(cur->args) - count_trailing_redirects(cur->args,
			ft_strlen_tab(cur->args)) == 1)
	{
		ft_print_env_export(tmp_export, cur->args);
		return (1);
	}
	while (cur->args[i])
	{
		var_check = ft_get_pvar(cur->args[i]);
		if (var_check)
		{
			var = var_check;
			path = my_getenv_lst(var, data->env);
			if (path)
			{
				value = ft_get_value(cur->args[i]);
				p_var = ft_get_pvar(cur->args[i]);
				p_res = ft_strjoin(ft_get_var_and_value(path, data->env),
						value);
				free(value);
				// printf("path %s| p_var: %s\n", path, p_var);
				ft_modif_env_var(tmp, data->env, p_var, p_res);
				ft_modif_env_var(tmp_export, data->export_env, p_var, p_res);
				free(path);
				free(p_res);
				free(p_var);
			}
			free(var_check);
		}
		var_check = ft_get_var(cur->args[i]);
		if (!var_check)
		{
			var = cur->args[i];
			path = my_getenv_lst(var, data->export_env);
			if (!path)
			{
				node = (t_lst *)ft_lstnew_generic(sizeof(t_lst));
				if (!node)
					return (0);
				node->str = ft_strdup(cur->args[i]);
				ft_lstadd_back_generic((void **)&data->export_env, node,
					sizeof(char *));
			}
			free(path);
		}
		else if (ft_check_env_var(var_check))
		{
			var = ft_get_var(cur->args[i]);
			path = my_getenv_lst(var, data->env);
			// printf("path %s| var: %s\n", path, var);
			if (!path)
			{
				node = (t_lst *)ft_lstnew_generic(sizeof(t_lst));
				if (!node)
					return (0);
				node->str = ft_strdup(cur->args[i]);
				ft_lstadd_back_generic((void **)&data->env, node,
					sizeof(char *));
				node_export = (t_lst *)ft_lstnew_generic(sizeof(t_lst));
				if (!node_export)
					return (0);
				node_export->str = ft_strdup(cur->args[i]);
				ft_lstadd_back_generic((void **)&data->export_env, node_export,
					sizeof(char *));
			}
			else
			{
				ft_modif_env_var(tmp, data->env, var, cur->args[i]);
				ft_modif_env_var(tmp_export, data->export_env, var,
					cur->args[i]);
			}
			free(path);
			free(var);
		}
		free(var_check);
		i++;
	}
	return (1);
}
