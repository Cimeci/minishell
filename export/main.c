/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/09 17:40:10 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	if (table)
	{
		while (table[i])
			free(table[i++]);
		free(table);
		table = NULL;
	}
}

static char	*my_getenv(const char *name, char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] != '=' && env[i][j])
			j++;
		tmp = ft_substr(env[i], 0, j);
		if (ft_strncmp(tmp, name, j) == 0 && name[j] == '\0')
		{
			free(tmp);
			return (env[i] + j + 1);
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

char	*find_pathname(char *cmd, char **cmd_split, char **path_split, int i)
{
	char	*pathname;

	cmd = ft_strjoin("/", cmd_split[0]);
	pathname = ft_strjoin(path_split[i], cmd);
	free(cmd);
	if (!access(pathname, F_OK | X_OK))
	{
		ft_free_tab(path_split);
		ft_free_tab(cmd_split);
		return (pathname);
	}
	free(pathname);
	return (NULL);
}

char	*find_path(char **env, char *cmd)
{
	char	**cmd_split;
	char	**path_split;
	char	*pathname;
	char	*tmp;
	int		i;

	i = 0;
	tmp = my_getenv("PATH", env);
	if (!tmp)
		return (NULL);
	if (ft_strncmp(cmd, "./", 2) == 0 || !access(cmd, X_OK))
		return (cmd);
	path_split = ft_split(tmp, ':');
	cmd_split = ft_split(cmd, ' ');
	while (path_split[i])
	{
		pathname = find_pathname(cmd, cmd_split, path_split, i);
		if (pathname)
			return (pathname);
		i++;
	}
	ft_free_tab(path_split);
	ft_free_tab(cmd_split);
	return (NULL);
}

int	ft_lstsize2(t_env *lst)
{
	int	i;

	i = 0;
	if (!lst)
		return (0);
	if (lst->next == NULL)
		return (1);
	while (lst->next != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i + 1);
}

t_env	*ft_lstnew2(void *path)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (new == NULL)
		return (0);
	new->path = path;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back2(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return ;
	temp = *lst;
	if (!temp)
	{
		*lst = new;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

t_env	*ft_init_env(char **env)
{
	t_env	*s_env;
	t_env	*list;
	int		i;

	i = 0;
	list = NULL;
	while (env[i])
	{
		s_env = malloc(sizeof(t_env));
		s_env->path = ft_strdup(env[i++]);
		s_env->next = NULL;
		ft_lstadd_back2(&list, s_env);
	}
	return (list);
}

static char	*my_getenv_lst(const char *name, t_env *env)
{
	int		j;
	char	*tmp;
	t_env	*cur;

	cur = env;
	while (cur)
	{
		j = 0;
		while (cur->path[j] != '=' && cur->path[j])
			j++;
		tmp = ft_substr(ft_strdup(cur->path), 0, j);
		if (!ft_strncmp(tmp, name, j))
		{
			return (tmp);
		}
		free(tmp);
		cur = cur->next;
	}
	return (NULL);
}

void	execution_cmd(char **argv, char **env)
{
	char	*path;
	pid_t	pid;

	path = find_path(env, argv[0]);
	if (!path)
	{
		perror("Error");
		return ;
	}
	if (path && access(path, X_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			env = NULL;
			execve(path, argv, env);
		}
		else
		{
			if (wait(NULL) == -1)
				perror("wait failed");
		}
	}
	free(path);
}

char	**ft_convert_lst_to_tab(t_env *env)
{
	int		i;
	char	**table;
	t_env	*tmp;

	i = 0;
	table = malloc(sizeof(char *) * (ft_lstsize2(env) + 1));
	tmp = env;
	while (tmp)
	{
		table[i] = tmp->path;
		i++;
		tmp = tmp->next;
	}
	table[i] = NULL;
	return (table);
}

char	*ft_get_var(char *str)
{
	char	*var;
	int		i;

	i = 0;
	while (str[i] != '=' && str[i])
		i++;
	var = ft_substr(str, 0, i);
	return (var);
}

t_env	*ft_export(char **argv, t_env *env)
{
	char	*path;
	char	*var;
	t_env	*cur;

	cur = env;
	if (!ft_strncmp(argv[0], "export", ft_strlen(argv[0])) && argv[1])
	{
		var = ft_get_var(argv[1]);
		path = my_getenv_lst(argv[1], env);
		// printf("var: %s | path: %s\n", var, path);
		if (!path)
			ft_lstadd_back2(&env, ft_lstnew2(ft_strdup(argv[1])));
		else
		{
			free(path);
			path = NULL;
			while (cur)
			{
				path = my_getenv_lst(cur->path, env);
				// printf("var: %s | path: %s\n", var, path);
				if (!ft_strncmp(var, path, ft_strlen(var)))
				{
					free(var);
					free(path);
					free(cur->path);
					cur->path = NULL;
					// printf("%s\n", argv[1]);
					cur->path = ft_strdup(argv[1]);
					break ;
				}
				free(path);
				cur = cur->next;
			}
		}
	}
	else
		execution_cmd(argv, ft_convert_lst_to_tab(cur));
	return (env);
}

void	ft_lstclear2(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (env)
	{
		temp = (env)->next;
		if (env->path)
			free(env->path);
		if (env->path)
			free(env);
		env = temp;
	}
	temp = NULL;
}

int	main(int argc, char **argv, char **env)
{
	char *input;
	t_env *list;

	if (argc != 1)
	{
		printf("Error input\n");
		exit(1);
	}
	input = "1";
	list = ft_init_env(env);
	while (input)
	{
		input = readline("Minishell> ");
		if (input && input[0])
		{
			add_history(input);
			argv = ft_split(input, ' ');
			if (argv)
			{
				list = ft_export(argv, list);
				ft_free_tab(argv);
				argv = NULL;
			}
		}
		if (input)
			free(input);
	}
	// ft_lstclear2(list);
}