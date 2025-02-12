/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noacharbogne <noacharbogne@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:22:29 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/12 15:29:15 by noacharbogn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		exit(130);
	}
}

// void	child_signal_handler_re(int signum)
// {
// 	if (signum == SIGINT)
// 	{
// 		write(1, "\n", 1);
// 	}
// }

void	parent_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
