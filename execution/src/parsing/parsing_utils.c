/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:40:02 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/20 19:04:12 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(t_data *data, char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
		{
			i++;
			while (input[i] && input[i] != '\'')
				i++;
			if (input[i] != '\'')
				return (errors(data, "'", ERROR_SYNTAX));
		}
		if (input[i] == '"')
		{
			i++;
			while (input[i] && input[i] != '"')
				i++;
			if (input[i] != '"')
				return (errors(data, "\"", ERROR_SYNTAX));
		}
		i++;
	}
	return (1);
}

void	get_shell_pid(t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return ;
	if (pid == 0)
		exit(0);
	else
	{
		data->shell_pid = ft_itoa(pid);
		if (!data->shell_pid)
		{
			perror("ft_itoa failed");
			exit(1);
		}
	}
	wait(NULL);
	return ;
}

int	is_white_space(char c)
{
	if (c == ' ' || (c >= 7 && c <= 13))
		return (1);
	return (0);
}
