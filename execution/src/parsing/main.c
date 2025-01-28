/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/28 09:45:52 by ncharbog         ###   ########.fr       */
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
		data->gexit_code = 1;
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
		//int	i = 0;
		// t_cmd *cur;
		// cur = data->cmd;
		// while (cur)
		// {
		// 	printf("cmd : %s\n", cur->cmd);
		// 	i = 0;
		// 	while (cur->args[i])
		// 	{
		// 		printf("args[%d] = %s\n", i, cur->args[i]);
		// 		i++;
		// 	}
		// 	i = 0;
		// 	while (cur->outfile && cur->outfile[i])
		// 	{
		// 		printf("outfile[%d] : %s\n", i, cur->outfile[i]);
		// 		i++;
		// 	}
		// 	i = 0;
		// 	while (cur->infile && cur->infile[i])
		// 	{
		// 		printf("infile[%d] : %s\n", i, cur->infile[i]);
		// 		i++;
		// 	}
		// 	i = 0;
		// 	while (cur->heredoc && cur->heredoc[i])
		// 	{
		// 		printf("heredoc[%d] = %s\n", i, cur->heredoc[i]);
		// 		i++;
		// 	}
		// 	i = 0;
		// 	while (cur->flag_redir && cur->flag_redir[i])
		// 	{
		// 		printf("flag_redir[%d] = %d\n", i, cur->flag_redir[i]);
		// 		i++;
		// 	}
		// 	cur = cur->next;
		// }
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

	// signal(SIGINT, parent_signal_handler);
	// signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("$> ");
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
