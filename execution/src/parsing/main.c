/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/18 09:56:10 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	parsing(t_data *data, char *input)
{
	t_cmd	*cur;

	cur = NULL;
	if (!check_quotes(data, input))
		return (0);
	data->env_cp = ft_convert_lst_to_tab(data->env);
	data->line = input;
	tokenise(data);
	if (data->token)
	{
		get_cmds(data);
		cur = data->cmd;
		while (cur)
		{
			handle_here_doc(data, cur);
			setup_child_process(data, cur);
			cur = cur->next;
		}
		if (!check_syntax(data))
			return (0);
	}
	else
		return (0);
	return (1);
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
	data->env_cp = NULL;
	data->fd[0] = -1;
	data->fd[1] = -1;
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
}

void	prompt(t_data *data)
{
	char	*input;
	char	*user_read;

	signal(SIGINT, parent_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		if (g_exit_code_sig)
		{
			data->gexit_code = g_exit_code_sig;
			g_exit_code_sig = 0;
		}
		data->pwd = ft_find_pwd(data);
		user_read = ft_strjoin(data->pwd, "$ ");
		// rl_outstream = stderr; // gere ./minishell | cat -e
		input = readline(user_read);
		free(user_read);
		if (!input)
		{
			printf("exit\n");
			return ;
		}
		if (input[0] == '\0')
			free(input);
		else if ((input[0] == '!' || input[0] == ':') && ft_strlen(input) == 1)
		{
			if (input[0] == '!')
				data->gexit_code = 1;
		}
		else
		{
			add_history(input);
			if (parsing(data, input))
				exec(data);
			free_all(data, 1);
		}
	}
	rl_clear_history();
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	int		last_exit;

	(void)argv;
	if (argc != 1)
	{
		printf("too many arguments\n");
		return (1);
	}
	init_data(&data, env);
	prompt(&data);
	last_exit = data.gexit_code;
	free_all(&data, 0);
	exit(last_exit);
}
