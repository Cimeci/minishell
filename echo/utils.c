/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 10:28:32 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 10:31:27 by inowak--         ###   ########.fr       */
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

int	count_file(char **argv)
{
	int	file;
	int	i;

	file = 0;
	i = 0;
	while (argv[i])
	{
		if (!ft_strncmp(">", argv[i], ft_strlen(argv[i])) || !ft_strncmp(">>",
				argv[i], ft_strlen(argv[i])))
			file++;
		i++;
	}
	return (file);
}

void	execution_cmd(char **argv, char **env)
{
	char	*path;
	pid_t	pid;

	path = ft_strjoin("/usr/bin/", argv[0]);
	if (!path)
		return ;
	if (path && access(path, X_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		else if (pid == 0)
		{
			execve(path, argv, env);
			perror("Error");
		}
		else
		{
			if (wait(NULL) == -1)
				perror("wait failed");
		}
	}
	free(path);
}
