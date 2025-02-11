/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:22:29 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/11 09:15:29 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_code_sig = 0;

void	child_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		g_exit_code_sig = 130;
		// printf("%d\n", g_exit_code_sig);
		exit(130);
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