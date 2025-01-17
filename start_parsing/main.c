/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/17 10:42:54 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	parsing(t_data *data)
{
	if (check_quotes(data) == -1)
	{
		printf("erreur quote");
		exit(EXIT_FAILURE);
	}
	env_variables(data);
	tokenise(data);
	//get_cmds(data);
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
		input = readline("minishell> ");
		if (input)
		{
			add_history(input);
			data->line = input;
			parsing(data);
			free(input);
		}
		t_cmd	*cur;
		int		i = 0;
		cur = data->cmd;
		while (cur)
		{
			printf("cmd : %s\n", cur->cmd);
			i = 0;
			while (cur->args[i])
			{
				printf("args : %s\n", cur->args[i]);
				i++;
			}
			printf("outfile : %s\n", cur->outfile);
			printf("infile : %s\n", cur->infile);
			cur = cur->next;
			printf ("\n");
		}
		free_all(data);
	}
}

int main(int argc, char **env)
{
	t_data	data;

	if (argc != 1)
	{
		printf("too many arguments\n");
		return (1);
	}
	init_data(&data, env);
	prompt(&data);
	free_all(&data);
}
