/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/28 11:11:43 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execution_cmd(t_cmd *cur, char *file)
{
	char	**args;
	int		i;
	
	i = 0;
	args = malloc(sizeof(char *) * (ft_strlen_tab(cur->args) + 2));
	while (cur->args[i])
	{
		args[i] = ft_strdup(cur->args[i]);
		i++;
	}
	args[i] = ft_strdup(file);
	i++;
	args[i] = NULL;
	ft_free_tab(cur->args);
	cur->args = NULL;
	cur->args = malloc(sizeof(char *) * (i + 1));
	if (!cur->args)
		return ;
	i = 0;
	while (args[i])
	{
		cur->args[i] = ft_strdup(args[i]);
		i++;
	}
	cur->args[i] = NULL;
	ft_free_tab(args);
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
		free(input);
	}
}

void	ft_heredoc(t_data *data, t_cmd *cur)
{
	char	*file;
	int		fd;

	(void)data;
	file = randomizer();
	// printf("file :%s\n", file);
	fd = open(file, O_TRUNC | O_CREAT | O_WRONLY, 0664);
	if (fd < 0)
	{
		printf("Error fd\n");
		return ;
	}
	if (access(file, F_OK) == -1)
	{
		printf("Error file not find\n");
		return ;
	}
	write_tmpfile(fd);
	execution_cmd(cur, file);
	close(fd);
	free(file);
}
