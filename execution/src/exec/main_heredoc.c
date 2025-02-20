/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/20 16:54:16 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	extension_write(t_data *data, t_cmd *cur, char *input, int fd)
{
	char	*input_cpy;

	input_cpy = NULL;
	if (cur->expand == true)
	{
		input_cpy = env_variables(data, ft_strdup(input), true);
		ft_putendl_fd(input_cpy, fd);
		free(input_cpy);
	}
	else
		ft_putendl_fd(input, fd);
}

void	write_tmpfile(t_data *data, t_cmd *cur, int fd)
{
	char	*input;
	int		original_in;
	int		i;

	i = 0;
	original_in = dup(STDIN_FILENO);
	signal(SIGINT, child_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (cur->heredoc[i])
	{
		while (1)
		{
			input = readline("> ");
			if (g_exit_code_sig == 130)
			{
				if (input)
					free(input);
				dup2(original_in, STDIN_FILENO);
				close(original_in);
				return ;
			}
			if (!input)
			{
				data->gexit_code = 0;
				break ;
			}
			if (!ft_strncmp(input, cur->heredoc[i], ft_strlen(input))
				&& ft_strlen(input) == ft_strlen(cur->heredoc[i]))
			{
				free(input);
				break ;
			}
			if (i == ft_strlen_tab(cur->heredoc) - 1)
				extension_write(data, cur, input, fd);
			free(input);
		}
		i++;
	}
	close(original_in);
}

void	ft_heredoc(t_data *data, t_cmd *cur)
{
	int	fd;

	if (cur->here == 1)
	{
		cur->file = randomizer();
		if (!cur->file)
		{
			data->gexit_code = 1;
			return ;
		}
		fd = open(cur->file, O_TRUNC | O_CREAT | O_WRONLY, 0664);
		if (fd < 0 || access(cur->file, F_OK) == -1)
		{
			printf("Error: heredoc's temporary file not found\n");
			return ;
		}
		write_tmpfile(data, cur, fd);
		signal(SIGINT, SIG_DFL);
		close(fd);
	}
	else
		return ;
}
