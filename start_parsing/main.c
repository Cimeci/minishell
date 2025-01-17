/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/17 16:23:11 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

void	parsing(t_data *data, char *input)
{
	if (check_quotes(input) == -1)
	{
		printf("erreur quote");
		return ;
	}
	env_variables(data, input);
	tokenise(data);
	if (data->token)
	{
		if (!check_syntax(data))
			return ;
		get_cmds(data);
	}
	else
		return ;
}

void	init_data(t_data *data, char **env)
{
	int		i;
	t_lst	*cur;

	i = 0;
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
