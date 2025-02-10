/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:22:29 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/10 14:27:56 by inowak--         ###   ########.fr       */
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