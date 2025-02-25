/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:36:53 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/25 09:53:26 by ncharbog         ###   ########.fr       */
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
		if (last_token->type == PIPE && last_token->next->type == PIPE)
			return (errors(data, last_token->str, ERROR_SYNTAX));
		last_token = last_token->next;
	}
	if (last_token->next == NULL && last_token->type <= 3)
		return (errors(data, "newline", ERROR_SYNTAX));
	while (i++ < 5)
	{
		if (last_token->type == i)
			return (errors(data, last_token->str, ERROR_SYNTAX));
	}
	return (1);
}

int	launch_heredoc(t_data *data)
{
	t_cmd	*cur_cmd;
	t_token	*cur_tok;

	cur_cmd = data->cmd;
	cur_tok = data->token;
	if (cur_tok->type == PIPE)
		return (errors(data, "|", ERROR_SYNTAX));
	while (cur_cmd && g_exit_code_sig != 130)
	{
		while (cur_tok->next != NULL)
		{
			if (cur_tok->type == PIPE)
			{
				cur_tok = cur_tok->next;
				break ;
			}
			if (cur_tok->type == HEREDOC && cur_tok->next->type <= PIPE)
				return (errors(data, "<<", ERROR_SYNTAX));
			cur_tok = cur_tok->next;
		}
		ft_heredoc(data, cur_cmd);
		if (cur_tok->type == PIPE)
			return (errors(data, "|", ERROR_SYNTAX));
		cur_cmd = cur_cmd->next;
	}
	return (1);
}

int	parsing(t_data *data)
{
	t_cmd	*cur_cmd;

	cur_cmd = NULL;
	if (!check_quotes(data, data->line))
		return (0);
	data->env_cp = ft_convert_lst_to_tab(data->env);
	tokenise(data);
	if (data->token)
	{
		get_cmds(data);
		if (!launch_heredoc(data))
			return (0);
		signal(SIGINT, SIG_DFL);
		if (!check_syntax(data))
			return (0);
	}
	else
		return (0);
	return (1);
}
