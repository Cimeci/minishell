/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/10 17:33:39 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	reset_args(t_cmd *cur, char *file)
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

void	write_tmpfile(t_data *data, t_cmd *cur, int fd)
{
	char	*input;
	int		i;

	i = 0;
	signal(SIGINT, child_signal_handler);
	signal(SIGQUIT, SIG_IGN);
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
			{
				if (cur->expand == true)
					ft_putendl_fd(env_variables(data, ft_strdup(input), true), fd);
				else
					ft_putendl_fd(input, fd);
			}
			free(input);
		}
		i++;
	}
}

void	ft_heredoc(t_data *data, t_cmd *cur)
{
	int	fd;

	(void)data;
	fd = open(cur->file, O_TRUNC | O_CREAT | O_WRONLY, 0664);
	if (fd < 0)
	{
		printf("Error fd\n");
		return ;
	}
	if (access(cur->file, F_OK) == -1)
	{
		printf("Error tmpfile not find\n");
		return ;
	}
	write_tmpfile(data, cur, fd);
	signal(SIGINT, SIG_DFL);
	reset_args(cur, cur->file);
	close(fd);
}
