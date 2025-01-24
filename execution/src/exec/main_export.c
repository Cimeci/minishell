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
	int	argc;

	argc = ft_strlen(var) - 1;
	// printf("%s|%c|%c|\n", var, var[0], var[argc]);
	i = 1;
	if (!var)
		return (0);
	if (!ft_isalpha(var[0]))
	{
		if (var[0] != '_')
			return (0);
	}
	if (!ft_isalnum(var[argc]))
		return (0);
	while (i < argc)
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

int		ft_export_append(t_data *data, char *arg);
int		ft_export_assign(t_data *data, char *var, char *value);
int		ft_add_to_list(t_lst **list, char *arg);
void	ft_print_export_env(t_lst *export_env);
int		ft_export(t_data *data, t_cmd *cur);

int	ft_add_to_list(t_lst **list, char *arg)
{
	t_lst	*node;

	node = ft_lstnew_generic(sizeof(t_lst));
	if (!node)
		return (0);
	node->str = ft_strdup(arg);
	ft_lstadd_back_generic((void *)list, node, sizeof(char *));
	return (1);
}

void	ft_print_export_env(t_lst *export_env)
{
	while (export_env)
	{
		printf("%s\n", export_env->str);
		export_env = export_env->next;
	}
}

int	ft_export_append(t_data *data, char *arg)
{
	char	*var;
	char	*path;
	char	*current_value;
	char	*new_value;
	char	*full_var;

	var = ft_get_pvar(arg);
	path = my_getenv_lst(var, data->env);
	new_value = ft_get_value(arg);
	if (!path)
		return (ft_export_assign(data, var, new_value));
	current_value = ft_get_var_and_value(path, data->env);
	full_var = ft_strjoin(current_value, new_value);
	ft_modif_env_var(data->env, data->env, var, full_var);
	ft_modif_env_var(data->export_env, data->export_env, var, full_var);
	free(var);
	free(path);
	free(new_value);
	free(full_var);
	return (1);
}

int	ft_export_assign(t_data *data, char *var, char *value)
{
	char	*path;
	char	*full_arg;

	path = my_getenv_lst(var, data->env);
	full_arg = ft_strjoin(var, "=");
	full_arg = ft_strjoin(full_arg, value);
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
	char	*pwd;

	i = 1;
	if (ft_strlen_tab(cur->args) - count_trailing_redirects(cur->args,
			ft_strlen_tab(cur->args)) == 1)
	{
		ft_print_export_env(data->export_env);
		return (1);
	}
	while (cur->args[i])
	{
		if (!ft_strncmp(cur->args[i], "PWD", ft_strlen(cur->args[i]))
			&& ft_strlen(cur->args[i]) == 3)
		{
			pwd = ft_find_pwd();
			ft_export_assign(data, "PWD", pwd);
			free(pwd);
			return (0);
		}
		if (!ft_check_env_var(ft_get_var(cur->args[i])))
		{
			if (!ft_check_env_var(ft_get_pvar(cur->args[i])))
				perror("Export Error\n");
		}
		if (ft_get_pvar(cur->args[i]))
		{
			if (!ft_export_append(data, cur->args[i]))
				return (0);
		}
		else if (ft_get_var(cur->args[i]))
		{
			var = ft_get_var(cur->args[i]);
			value = ft_get_value(cur->args[i]);
			if (!ft_export_assign(data, var, value))
				return (0);
			free(var);
			free(value);
		}
		else if (!ft_add_to_list(&data->export_env, cur->args[i]))
			return (0);
		i++;
	}
	return (1);
}
