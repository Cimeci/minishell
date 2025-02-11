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
	int	len;

	if (!var)
		return (1);
	len = ft_strlen(var) - 1;
	i = 1;
	if (!ft_isalpha(var[0]))
	{
		if (var[0] != '_')
		{
			errors_exec(var, "export", IDENTIFIER);
			return (1);
		}
	}
	if (!ft_isalnum(var[len]))
	{
		errors_exec(var, "export", IDENTIFIER);
		return (1);
	}
	while (i < len)
	{
		if (!ft_isalnum(var[i]))
		{
			if (var[i] != '_')
			{
				errors_exec(var, "export", IDENTIFIER);
				return (1);
			}
		}
		i++;
	}
	return (0);
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

int	ft_add_to_list(t_lst **list, char *arg)
{
	t_lst	*node;

	node = ft_lstnew_generic(sizeof(t_lst));
	if (!node)
		return (0);
	node->str = ft_strdup(arg);
	ft_lstadd_back_generic((void **)list, node, sizeof(char *));
	return (1);
}

void	ft_print_export_env(t_lst *export_env)
{
	t_lst	*tmp;

	tmp = export_env;
	while (tmp)
	{
		printf("%s\n", tmp->str);
		tmp = tmp->next;
	}
}

int	ft_export_append(t_data *data, char *arg)
{
	char	*var;
	char	*path;
	char	*current_value;
	char	*new_value;
	char	*full_var;

	full_var = NULL;
	current_value = NULL;
	var = ft_get_pvar(arg);
	path = my_getenv_lst(var, data->env);
	new_value = ft_get_value(arg);
	if (!path)
	{
		ft_export_assign(data, var, new_value);
	}
	else
	{
		current_value = ft_get_var_and_value(path, data->env);
		full_var = ft_strjoin(current_value, new_value);
		ft_modif_env_var(data->env, data->env, var, full_var);
		ft_modif_env_var(data->export_env, data->export_env, var, full_var);
		free(full_var);
	}
	free(var);
	free(path);
	free(new_value);
	return (1);
}

int	ft_export_assign(t_data *data, char *var, char *value)
{
	char	*path;
	char	*full_arg;
	char	*var_egal;

	path = my_getenv_lst(var, data->env);
	var_egal = ft_strjoin(var, "=");
	full_arg = ft_strjoin(var_egal, value);
	free(var_egal);
	if (!path)
	{
		if (!ft_add_to_list(&data->env, full_arg))
			return (0);
		if (!ft_add_to_list(&data->export_env, full_arg))
			return (0);
	}
	else
	{
		ft_modif_env_var(data->env, data->env, var, full_arg);
		ft_modif_env_var(data->export_env, data->export_env, var, full_arg);
		free(path);
	}
	free(full_arg);
	return (1);
}

int	ft_export(t_data *data, t_cmd *cur)
{
	int		i;
	char	*var;
	char	*value;
	char	*pvar;

	i = 1;
	if (ft_strlen_tab(cur->args) - count_trailing_redirects(cur->args,
			ft_strlen_tab(cur->args)) == 1)
	{
		ft_print_export_env(data->export_env);
		return (1);
	}
	while (cur->args[i])
	{
		var = ft_get_var(cur->args[i]);
		pvar = ft_get_pvar(cur->args[i]);
		data->gexit_code = 0;
		if (pvar)
		{
			if (ft_check_env_var(pvar))
			{
				data->gexit_code = 1;
				free(pvar);
				free(var);
				return (1);
			}
			ft_export_append(data, cur->args[i]);
			free(pvar);
			free(var);
		}
		else if (var)
		{
			if (ft_check_env_var(var))
			{
				data->gexit_code = 1;
				free(var);
				return (1);
			}
			value = ft_get_value(cur->args[i]);
			if (!ft_export_assign(data, var, value))
			{
				free(var);
				return (1);
			}
			free(value);
			free(var);
		}
		else if (ft_check_env_var(cur->args[i]))
		{
			data->gexit_code = 1;
			return (1);
		}
		else
		{
			if (!ft_add_to_list(&data->export_env, cur->args[i]))
				return (1);
		}
		i++;
	}
	return (0);
}
