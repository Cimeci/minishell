/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/20 17:21:20 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	child_pid = -1;

void	child_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		exit(0);
	}
}

void	parent_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (child_pid <= 0)
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}

static void	handle_child_process(void)
{
	char	*child_input;

	signal(SIGINT, child_signal_handler);
	child_input = "1";
	while (child_input)
	{
		child_input = readline("Heredoc> ");
		if (child_input)
			free(child_input);
	}
}

static void	handle_run_command(void)
{
	child_pid = fork();
	if (child_pid < 0)
	{
		perror("Erreur lors du fork");
		return ;
	}
	if (child_pid == 0)
		handle_child_process();
	else
	{
		waitpid(child_pid, NULL, 0);
		child_pid = -1;
	}
}

static void	handle_exit_command(char *input)
{
	if (child_pid > 0)
	{
		kill(child_pid, SIGINT);
		waitpid(child_pid, NULL, 0);
	}
	free(input);
	clear_history();
}

static void	process_command(char *input)
{
	if (!strcmp(input, "run"))
		handle_run_command();
	else if (!strcmp(input, "exit"))
	{
		handle_exit_command(input);
		exit(0);
	}
	else
		printf("Commande inconnue : %s\n", input);
}

int	main(void)
{
	char	*input;

	input = "1";
	signal(SIGINT, parent_signal_handler);
	while (input)
	{
		input = readline("minishell> ");
		if (input)
		{
			add_history(input);
			process_command(input);
			if (strcmp(input, "exit") != 0)
				free(input);
		}
	}
	return (0);
}
