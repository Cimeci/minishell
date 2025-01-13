/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 09:53:58 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define BUFFER_SIZE 100000

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
			return (NULL);
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

void	ft_absolut_path(char **argv, char **env, char *root)
{
	char	*path;
	char	*target_path;

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
		if (chdir(target_path))
		{
			ft_free_path(path, target_path);
			perror("Error");
			return ;
		}
		if (path)
			ft_free_path(path, target_path);
	}
}

void	ft_cd(char **argv, char **env)
{
	char	*root;

	root = NULL;
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
					perror("Error");
				return ;
			}
		}
	}
	ft_absolut_path(argv, env, root);
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
