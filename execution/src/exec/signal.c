/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:22:29 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/17 10:09:09 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_code_sig = 0;

void	child_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		exit(130);
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

void	parent_signal_handler_here(int signum)
{
	if (signum == SIGINT)
		g_exit_code_sig = 130;
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
