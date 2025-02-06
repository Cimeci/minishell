/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/06 15:23:24 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_syntax(t_data *data)
{
	t_token	*last_token;
	int		i;

	i = 0;
	last_token = data->token;
	if (data->token->type == PIPE)
		return (errors(data, "|", ERROR_SYNTAX));
	while (last_token->next != NULL)
	{
		if (last_token->type <= 3 && last_token->next->type == PIPE)
			return (errors(data, "|", ERROR_SYNTAX));
		if (last_token->type <= 3 && last_token->next->type <= 3)
			return (errors(data, last_token->str, ERROR_SYNTAX));
		last_token = last_token->next;
	}
	if (last_token->next == NULL && last_token->type <= 3)
		return (errors(data, "newline", ERROR_SYNTAX));
	while (i < 5)
	{
		if (last_token->type == i)
			return (errors(data, last_token->str, ERROR_SYNTAX));
		i++;
	}
	return (1);
}

void	parsing(t_data *data, char *input)
{
	if (!check_quotes(data, input))
		return ;
	data->line = ft_strdup(input);
	tokenise(data);
	if (data->token)
	{
		if (!check_syntax(data))
			return ;
		get_cmds(data);
		if (data->cmd->cmd
			&& (data->cmd->cmd[0] == '!' || data->cmd->cmd[0] == ':'))
		{
			if (data->cmd->cmd[0] == '!')
				data->gexit_code = 1;
			return ;
		}
	}
	else
		return ;
}

void	init_data(t_data *data, char **env)
{
	int		i;
	t_lst	*cur;
	char	buffer[BUFFER_SIZE];
	char	*check;

	i = 0;
	check = getcwd(buffer, BUFFER_SIZE);
	if (check)
		data->pwd = NULL;
	else
		perror("Error getcwd");
	get_shell_pid(data);
	data->line = NULL;
	data->token = NULL;
	data->cmd = NULL;
	data->env = NULL;
	data->gexit_code = 0;
	cur = data->env;
	while (env[i])
	{
		cur = (t_lst *)ft_lstnew_generic(sizeof(t_lst));
		if (!cur)
			return ;
		cur->str = ft_strdup(env[i]);
		cur->next = NULL;
		ft_lstadd_back_generic((void **)&data->env, cur, sizeof(char *));
		i++;
	}
	data->export_env = ft_dup_lst(data->env);
}

void	prompt(t_data *data)
{
	char	*input;
	char	*user_read;

	signal(SIGINT, parent_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		data->pwd = ft_find_pwd();
		user_read = ft_strjoin(data->pwd, "$ ");
		input = readline(user_read);
		free(user_read);
		if (!input)
		{
			printf("exit\n");
			return ;
		}
		if (input[0] == '\0')
			free(input);
		else
		{
			add_history(input);
			parsing(data, input);
			exec(data);
			free(input);
			free_all(data, 1);
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argv;
	if (argc != 1)
	{
		printf("too many arguments\n");
		return (1);
	}
	init_data(&data, env);
	prompt(&data);
	free_all(&data, 0);
}
