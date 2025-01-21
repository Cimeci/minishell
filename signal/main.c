/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/21 10:21:07 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t		g_child_pid = -1;

void	child_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		exit(1);
	}
}

void	parent_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (g_child_pid <= 0)
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}

static void	handle_exit_command(char *input)
{
	if (g_child_pid > 0)
	{
		kill(g_child_pid, SIGINT);
		waitpid(g_child_pid, NULL, 0);
	}
	free(input);
	clear_history();
}

static void	handle_child_process(void)
{
	char	*child_input;

	signal(SIGINT, child_signal_handler);
	child_input = NULL;
	while (1)
	{
		child_input = readline("Heredoc> ");
		if (!child_input)
		{
			exit(1);
		}
		if (!ft_strncmp(child_input, "exit", 4) && ft_strlen(child_input) == 4)
		{
			handle_exit_command(child_input);
			exit(0);
		}
		printf("%s\n", child_input);
		free(child_input);
	}
}

static void	handle_run_command(void)
{
	int	status;

	g_child_pid = fork();
	if (g_child_pid < 0)
	{
		perror("Erreur lors du fork");
		return ;
	}
	if (g_child_pid == 0)
		handle_child_process();
	else
	{
		waitpid(g_child_pid, &status, 0);
		g_child_pid = -1;
	}
}

static void	process_command(char *input)
{
	if (!ft_strncmp(input, "run", 3) && ft_strlen(input) == 3)
		handle_run_command();
	else if (!ft_strncmp(input, "exit", 4) && ft_strlen(input) == 4)
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

	signal(SIGINT, parent_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			exit(0);
		}
		if (*input)
			add_history(input);
		process_command(input);
		if (strcmp(input, "exit") != 0)
			free(input);
	}
	return (0);
}
