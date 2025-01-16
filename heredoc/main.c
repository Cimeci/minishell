/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/16 09:15:24 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	while (table[i])
		free(table[i++]);
	free(table);
	table = NULL;
}

char	*my_getenv(const char *name, char **env)
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

void	ft_lstadd_back3(t_save **lst, t_save *new)
{
	t_save	*temp;

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

void	ft_lstclear3(t_save *save)
{
	t_save	*temp;

	temp = save;
	while (save)
	{
		temp = save->next;
		if (save->line)
			free(save->line);
		free(save);
		save = temp;
	}
}

t_save	*ft_init_save(void)
{
	t_save	*save;

	save = malloc(sizeof(t_save));
	return (save);
}

void	ft_heredoc(char **argv, char **env)
{
	char	*input;
	t_save	*save;
	t_save	*cur;
	t_save	*tmp;
	int		info1;
	int		info2;
	char	*path;

	path = find_path(env, argv[0]);
	info1 = access(path, X_OK);
	info2 = ft_strncmp("<<", argv[1], ft_strlen(argv[1]));
	// printf("%s | %s | %d | %d\n", argv[0], path, info1, info2);
	if (info1 || info2 || !argv[2])
	{
		printf("Error\n");
		return ;
	}
	input = "1";
	save = NULL;
	cur = NULL;
	while (input)
	{
		input = readline("> ");
		if (!ft_strncmp(input, argv[2], ft_strlen(argv[2])))
			break ;
		if (input)
		{
			cur = malloc(sizeof(t_save));
			cur->line = ft_strdup(input);
			cur->next = NULL;
			ft_lstadd_back3(&save, cur);
		}
	}
	tmp = save;
	while (tmp)
	{
		printf("%s\n", tmp->line);
		tmp = tmp->next;
	}
	// ft_lstclear3(save);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;

	if (argc != 1)
	{
		printf("Error input\n");
		exit(1);
	}
	input = "1";
	while (input)
	{
		input = readline("Minishell> ");
		if (input)
		{
			add_history(input);
			argv = ft_split(input, ' ');
			if (argv)
			{
				ft_heredoc(argv, env);
				ft_free_tab(argv);
				argv = NULL;
			}
			free(input);
		}
	}
}
