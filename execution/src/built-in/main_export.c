/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/15 16:48:56 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_modif_env_var(t_env *cur, t_env *env, char *var_line)
{
	char	*path;
	char	*var;

	while (cur)
	{
		var = ft_get_var(cur->path);
		path = my_getenv_lst(var, env);
		if (!ft_strncmp(path, var, ft_strlen(path)))
		{
			free(path);
			free(var);
			free(cur->path);
			cur->path = ft_strdup(var_line);
			break ;
		}
		free(path);
		free(var);
		cur = cur->next;
	}
}

void	ft_print_env_export(t_env *env, char **argv)
{
	t_env	*cur;
	int		fd;

	fd = open_output_file(argv, ft_strlen_tab(argv));
	cur = env;
	while (cur)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putendl_fd(cur->path, fd);
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

char	*ft_get_var_and_value(char *var, t_env *env)
{
	int		j;
	char	*tmp;
	char	*tmp_dup;
	t_env	*cur;

	cur = env;
	while (cur)
	{
		j = 0;
		while (cur->path[j] != '=' && cur->path[j])
			j++;
		tmp_dup = ft_strdup(cur->path);
		tmp = ft_substr(tmp_dup, 0, j);
		if (!ft_strncmp(tmp, var, ft_strlen(var)))
		{
			free(tmp_dup);
			free(tmp);
			return (cur->path);
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

int	ft_export(char **argv, t_env *env, t_env *export_env)
{
	char	*path;
	char	*var;
	char	*var_check;
	char	*p_var;
	char	*value;
	t_env	*cur;
	t_env	*cur_export;
	int		i;

	i = 1;
	cur = env;
	cur_export = export_env;
	if (!ft_strncmp(argv[0], "export", ft_strlen(argv[0])))
	{
		if (ft_strlen_tab(argv) - count_trailing_redirects(argv, ft_strlen_tab(argv)) == 1)
		{
			ft_print_env_export(cur_export, argv);
			return (1);
		}
		while (argv[i])
		{
			var_check = ft_get_pvar(argv[i]);
			if (var_check)
			{
				var = var_check;
				path = my_getenv_lst(var, env);
				if (path)
				{
					value = ft_get_value(argv[i]);
					p_var = ft_strjoin(ft_get_var_and_value(path, env), value);
					free(value);
					// printf("path %s| p_var: %s\n", path, p_var);
					ft_modif_env_var(cur, env, p_var);
					ft_modif_env_var(cur_export, export_env, p_var);
					free(path);
					free(p_var);
				}
				free(var_check);
			}
			var_check = ft_get_var(argv[i]);
			if (!var_check)
			{
				var = argv[i];
				path = my_getenv_lst(var, export_env);
				if (!path)
					ft_lstadd_back2(&export_env,
						ft_lstnew2(ft_strdup(argv[i])));
				free(path);
			}
			else if (ft_check_env_var(var_check))
			{
				var = ft_get_var(argv[i]);
				path = my_getenv_lst(var, env);
				if (!path)
				{
					ft_lstadd_back2(&env, ft_lstnew2(ft_strdup(argv[i])));
					ft_lstadd_back2(&export_env,
						ft_lstnew2(ft_strdup(argv[i])));
				}
				else
				{
					ft_modif_env_var(cur, env, argv[i]);
					ft_modif_env_var(cur_export, export_env, argv[i]);
				}
				free(path);
				free(var);
			}
			free(var_check);
			i++;
		}
		return (1);
	}
	return (0);
}
