/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/21 17:16:01 by ncharbog         ###   ########.fr       */
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

void	exec(t_data *data)
{
	t_cmd	*cur;

	cur = data->cmd;
	while (cur)
	{
		// printf("|%s| |%s| |%zu|\n", cur->cmd, cur->args[0], ft_strlen(cur->args[0]));
		if (!ft_strncmp(cur->cmd, "cd", ft_strlen(cur->cmd)) && ft_strlen(cur->cmd) == 2)
			ft_cd(cur->args);
		else if (!ft_strncmp(cur->args[0], "pwd", ft_strlen(cur->args[0])) && ft_strlen(cur->args[0]) == 3)
			ft_pwd();
		else if (!ft_strncmp(cur->args[0], "echo", ft_strlen(cur->args[0])) && ft_strlen(cur->args[0]) == 4)
			ft_echo(ft_strlen_tab(cur->args), cur->args);
		else if (!ft_strncmp(cur->args[0], "env", ft_strlen(cur->args[0])) && ft_strlen(cur->args[0]) == 3)
			ft_env(data, cur);
		else if (!ft_strncmp(cur->args[0], "export", ft_strlen(cur->args[0])) && ft_strlen(cur->args[0]) == 6)
			ft_export(data, cur);
		cur = cur->next;
	}
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
		exec(data);
	}
	else
		return ;
}

void	init_data(t_data *data, char **env)
{
	int		i;
	t_lst	*cur;

	i = 0;
	get_shell_pid(data);
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

	input = "1";
	while (input)
	{
		input = readline("$> ");
		if (input)
		{
			add_history(input);
			if (input[0] == '\0')
				free(input);
			else
			{
				parsing(data, input);
				free(input);
			}
		}
		// t_cmd	*cur;
		// int		i = 0;
		// cur = data->cmd;
		// while (cur)
		// {
		// 	printf("cmd : %s\n", cur->cmd);
		// 	i = 0;
		// 	printf("outfile : %s\n", cur->outfile);
		// 	printf("infile : %s\n", cur->infile);
		// 	while (cur->args[i])
		// 	{
		// 		printf("args : %s\n", cur->args[i]);
		// 		i++;
		// 	}
		// 	cur = cur->next;
		// 	printf ("\n");
		// }
		free_all(data, 1);
	}
}

int main(int argc, char **argv, char **env)
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
