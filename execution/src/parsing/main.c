/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/24 10:42:53 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_syntax(t_data *data)
{
	t_token	*last_token;
	int		i;

	i = 0;
	last_token = data->token;
	while (last_token->next != NULL)
	{
		if (last_token->type <= 3 && last_token->next->type == PIPE)
		{
			printf("erreur de syntaxe\n");
			return (0);
		}
		if (last_token->type <= 3 && last_token->next->type <= 3)
		{
			printf("erreur de syntaxe\n");
			return (0);
		}
		last_token = last_token->next;
	}
	if (data->token->type == PIPE)
	{
		printf("erreur de syntaxe\n");
		return (0);
	}
	while (i < 5)
	{
		if (last_token->type == i)
		{
			printf("erreur de syntaxe\n");
			return (0);
		}
		i++;
	}
	return (1);
}

void	parsing(t_data *data, char *input)
{
	if (check_quotes(input) == -1)
	{
		printf("erreur quote");
		return ;
	}
	data->line = ft_strdup(input);
	env_variables(data);
	tokenise(data);
	if (data->token)
	{
		if (!check_syntax(data))
			return ;
		get_cmds(data);
		if (data->cmd->cmd[0] == '!' || data->cmd->cmd[0] == ':')
			return ;
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
		data->pwd = ft_strdup(buffer);
	else
		perror("Error getcwd");
	get_shell_pid(data);
	data->old_pwd = NULL;
	data->pwd = ft_strdup(buffer);
	data->line = NULL;
	data->token = NULL;
	data->cmd = NULL;
	data->env = NULL;
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
	char	*pwd;
	char	*user_read;

	signal(SIGINT, parent_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		pwd = ft_find_pwd();
		user_read = ft_strjoin(pwd, "$ ");
		input = readline(user_read);
		free(user_read);
		free(pwd);
		pwd = NULL;
		user_read = NULL;
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
