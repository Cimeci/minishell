/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 09:07:37 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define BUFFER_SIZE 100000

void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	while (table[i])
		free(table[i++]);
	free(table);
	table = NULL;
}

char	*ft_strsstr(char *pwd, char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] == pwd[i])
		i++;
	if (!cmd[i] || !pwd[i])
		return (cmd);
	return (NULL);
}

char	*ft_pwd(char **argv, char **env)
{
	char	buffer[BUFFER_SIZE];
	char	*path;

	if (!ft_strncmp(argv[0], "cd", ft_strlen(argv[0])))
	{
		if (!getcwd(buffer, BUFFER_SIZE))
		{
			perror("Error retrieving directory");
			return (NULL);
		}
		path = strdup(buffer);
		if (!path)
		{
			perror("Error allocating memory for path");
			return (NULL);
		}
		return (path);
	}
	if (!access(ft_strjoin("/usr/bin/", argv[0]), X_OK))
	{
		if (fork() == 0)
			execve(ft_strjoin("/usr/bin/", argv[0]), argv, env);
		else
			wait(NULL);
	}
	return (NULL);
}

void	ft_cd(char **argv, char **env)
{
	char	*root;
	char	*path;
	char	*target_path;

	if (!ft_strncmp(argv[0], "cd", ft_strlen(argv[0])))
	{
		if (!argv[1] || argv[1][0] == '\0')
		{
			ft_putendl_fd("Error : No path", 2);
			return ;
		}
		if (argv[1] && argv[1][0] != '\0')
		{
			if (!ft_strncmp(argv[1], "/", ft_strlen(argv[1])))
			{
				if (chdir("/"))
				{
					perror("Error");
					return ;
				}
				return ;
			}
		}
	}
	root = ft_pwd(argv, env);
	if (root)
	{
		path = NULL;
		target_path = ft_strsstr(root, argv[1]);
		if (target_path)
			free(root);
		if (!target_path)
		{
			path = ft_strjoin(root, "/");
			free(root);
			target_path = ft_strjoin(path, argv[1]);
		}
		printf("%s\n", target_path);
		if (chdir(target_path))
		{
			free(path);
			free(target_path);
			perror("Error");
			return ;
		}
		if (path)
		{
			free(path);
			free(target_path);
		}
	}
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
		if (input && input[0] != '\0')
		{
			add_history(input);
			argv = ft_split(input, ' ');
			if (argv)
			{
				ft_cd(argv, env);
				ft_free_tab(argv);
				argv = NULL;
			}
		}
		if (input)
			free(input);
	}
}
