/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:36:15 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/20 16:55:19 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_prompt(t_data *data)
{
	char	*user_read;

	signal(SIGINT, parent_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	data->pwd = ft_find_pwd(data);
	user_read = ft_strjoin(data->pwd, "$ ");
	data->line = readline(user_read);
	if (g_exit_code_sig)
	{
		data->gexit_code = g_exit_code_sig;
		g_exit_code_sig = 0;
	}
	free(user_read);
}

void	prompt(t_data *data)
{
	while (1)
	{
		read_prompt(data);
		if (!data->line)
		{
			printf("exit\n");
			break ;
		}
		if ((data->line[0] == '!' || data->line[0] == ':')
			&& ft_strlen(data->line) == 1)
		{
			if (data->line[0] == '!')
				data->gexit_code = 1;
		}
		else if (data->line[0] != '\0')
		{
			add_history(data->line);
			if (parsing(data) && g_exit_code_sig != 130)
				exec(data);
		}
		free_all(data, 1);
	}
	rl_clear_history();
}
