/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/18 14:02:06 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	extension_write(t_data *data, t_cmd *cur, char *input, int fd)
{
	char *input_cpy;

	input_cpy = ft_strdup(input);
	if (cur->expand == true)
		ft_putendl_fd(env_variables(data, input_cpy, true), fd);
	else
		ft_putendl_fd(input_cpy, fd);
	free(input_cpy);
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
					free (input);
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
	close(fd);
}
