/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/09 08:44:11 by ncharbog         ###   ########.fr       */
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

char	*get_path(const char *name, t_env *env)
{
	int		j;
	char	*tmp;

	while (env)
	{
		j = 0;
		while (env->path[j] != '=' && env->path[j])
			j++;
		tmp = ft_substr(env->path, 0, j);
		if (ft_strncmp(tmp, name, j) == 0 && name[j] == '\0')
		{
			free(tmp);
			return (env->path);
		}
		free(tmp);
		env = env->next;
	}
	return (NULL);
}

void	execution_cmd(char **argv, char **env)
{
	char	*path;
	pid_t	pid;

	path = ft_strjoin("/usr/bin/", argv[0]);
	if (!path)
	{
		ft_free_tab(argv);
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
			execve(path, argv, env);
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

t_env	*ft_unset(char **argv, t_env **env)
{
	t_env	*tmp;
	t_env	*prev;
	char	*unset_path;

	if (!ft_strncmp(argv[0], "unset", ft_strlen(argv[0])))
	{
		unset_path = get_path(argv[1], *env);
		if (!unset_path)
			return (NULL);
		tmp = *env;
		prev = NULL;
		while (tmp)
		{
			if (!ft_strncmp(tmp->path, unset_path, ft_strlen(tmp->path)))
			{
				if (prev == NULL)
					*env = tmp->next;
				else
					prev->next = tmp->next;
				free(tmp->path);
				free(tmp);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		return (*env);
	}
	else
		execution_cmd(argv, ft_convert_lst_to_tab(*env));
	return (NULL);
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
				list = ft_unset(argv, &list);
				ft_free_tab(argv);
				argv = NULL;
			}
		}
		if (input)
			free(input);
	}
}
