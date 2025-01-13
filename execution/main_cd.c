/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 15:59:27 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define BUFFER_SIZE 100000

char	*ft_find_pwd(void)
{
	char	buffer[BUFFER_SIZE];

	if (getcwd(buffer, BUFFER_SIZE) == NULL)
	{
		printf("Error directory\n");
		if (errno == ERANGE)
			printf("BUFFER_SIZE too small\n");
		return (NULL);
	}
	return (ft_strdup(buffer));
}

void	ft_absolut_path(char **argv, char *root)
{
	char	*path;
	char	*target_path;

	root = ft_find_pwd();
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
			printf("here\n");
			ft_free_path(path, target_path);
			perror("Error");
			return ;
		}
		if (path)
			ft_free_path(path, target_path);
	}
}

int	ft_cd(char **argv)
{
	char	*root;

	root = NULL;
	if (!ft_strncmp(argv[0], "cd", ft_strlen(argv[0])))
	{
		if (!argv[1] || argv[1][0] == '\0')
		{
			ft_putendl_fd("Error : No path", 2);
			return (0);
		}
		if (argv[1] && argv[1][0] != '\0')
		{
			if (!ft_strncmp(argv[1], "/", ft_strlen(argv[1])))
			{
				if (chdir("/"))
				{
					perror("Error");
					return (0);
				}
				return (1);
			}
			ft_absolut_path(argv, root);
			return (1);
		}
	}
	return (0);
}
