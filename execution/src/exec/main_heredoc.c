/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/28 15:48:21 by inowak--         ###   ########.fr       */
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
	args[i++] = ft_strdup(file);
	args[i] = NULL;
	ft_free_tab(cur->args);
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

void	write_tmpfile(t_cmd *cur, int fd)
{
	char	*input;
	int		i;

	i = 0;
	signal(SIGINT, child_signal_handler);
	while (cur->heredoc[i])
	{
		while (1)
		{
			input = readline("> ");
			if (!input)
				break ;
			if (!ft_strncmp(input, cur->heredoc[i], ft_strlen(input))
				&& ft_strlen(input) == ft_strlen(cur->heredoc[i]))
			{
				free(input);
				break ;
			}
			if (i == ft_strlen_tab(cur->heredoc) - 1)
				ft_putendl_fd(input, fd);
			free(input);
		}
		i++;
	}
}

void	ft_heredoc(t_data *data, t_cmd *cur)
{
	int	fd;

	(void)data;
	signal(SIGINT, SIG_IGN);
	fd = open(cur->file, O_TRUNC | O_CREAT | O_WRONLY, 0664);
	if (fd < 0)
	{
		printf("Error fd\n");
		return ;
	}
	if (access(cur->file, F_OK) == -1)
	{
		printf("Error file not find\n");
		return ;
	}
	write_tmpfile(cur, fd);
	signal(SIGINT, SIG_DFL);
	execution_cmd(cur, cur->file);
	close(fd);
}
