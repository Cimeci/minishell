/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/28 09:51:54 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execution_cmd(char **env, char *file)
{
	pid_t	pid;
	char	**args;

	args = malloc(sizeof(char *) * 3);
	args[0] = ft_strdup("cat");
	args[1] = ft_strdup(file);
	args[2] = NULL;
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
		execve("/usr/bin/cat", args, env);
	else if (wait(NULL) == -1)
	{
		perror("wait failed");
		exit(0);
	}
}

void	write_tmpfile(int fd)
{
	char	*input;

	while (1)
	{
		input = readline("heredoc > ");
		if (!input)
			break ;
		if (!ft_strncmp(input, "END", ft_strlen(input))
			&& ft_strlen(input) == ft_strlen("END"))
		{
			free(input);
			break ;
		}
		ft_putendl_fd(input, fd);
	}
	close(fd);
}

void	ft_heredoc(char **env)
{
	char	*file;
	int		fd;

	file = randomizer();
	fd = open(file, O_TRUNC | O_CREAT | O_WRONLY, 0664);
	write_tmpfile(fd);
	execution_cmd(env, file);
	unlink(file);
}
