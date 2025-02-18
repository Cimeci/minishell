/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:22:29 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/18 13:47:13 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_code_sig = 0;

void	child_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		g_exit_code_sig = 130;
		rl_on_new_line();
		rl_replace_line("", 0);
		close(STDIN_FILENO);
	}
}

void	parent_signal_handler_exec(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		g_exit_code_sig = 130;
	}
	if (signum == SIGQUIT)
	{
		write(1, "Quit\n", 5);
		g_exit_code_sig = 131;
	}
}

void	parent_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_code_sig = 130;
	}
}
