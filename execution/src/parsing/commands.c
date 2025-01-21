/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:38:56 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/21 18:15:19 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*redir_cmd(t_cmd *cur_cmd, t_token *cur_tok)
{
	if (cur_tok->type == INPUT && cur_tok->next != NULL)
	{
		cur_cmd->infile = ft_strdup(cur_tok->next->str);
		cur_tok = cur_tok->next->next;
	}
	else if (cur_tok->next != NULL && (cur_tok->type == OVERWRITE
		|| cur_tok->type == APPEND))
	{
		cur_cmd->outfile = ft_strdup(cur_tok->next->str);
		if (cur_tok->type == APPEND)
			cur_cmd->flag_redir = 1;
		cur_tok = cur_tok->next->next;
	}
	else
	{
		cur_tok = cur_tok->next;
		printf("erreur de syntaxe redir\n");
	}
	return (cur_tok);
}

t_token	*build_cmd(t_data *data, t_cmd *cur_cmd, t_token *cur_tok)
{
	t_token	*tmp;
	int		i;
	int		len;

	i = 0;
	len = 0;
	tmp = cur_tok;
	cur_cmd->cmd = find_path(data, cur_tok->str);
	// printf("|%s|%s|\n", cur_tok->str, cur_cmd->cmd);
	if (!cur_cmd->cmd)
		cur_cmd->cmd = ft_strdup(cur_tok->str);
	while (tmp && tmp->type != PIPE)
	{
		if (cur_tok->type == INPUT || cur_tok->type == APPEND
			|| cur_tok->type == OVERWRITE)
			tmp = tmp->next->next;
		len++;
		tmp = tmp->next;
	}
	cur_cmd->args = malloc((len + 1) * sizeof(char *));
	if (!cur_cmd->args)
		return (NULL);
	while (cur_tok && cur_tok->type != PIPE)
	{
		if (cur_tok->type == INPUT || cur_tok->type == APPEND
			|| cur_tok->type == OVERWRITE)
			cur_tok = redir_cmd(cur_cmd, cur_tok);
		else
		{
			cur_cmd->args[i] = ft_strdup(cur_tok->str);
			i++;
			cur_tok = cur_tok->next;
		}
	}
	cur_cmd->args[i] = NULL;
	return (cur_tok);
}

void	get_cmds(t_data *data)
{
	t_cmd	*cur_cmd;
	t_token	*cur_tok;

	cur_cmd = data->cmd;
	cur_tok = data->token;
	while (cur_tok)
	{
		cur_cmd = (t_cmd *)ft_lstnew_generic(sizeof(t_cmd));
		if (!cur_cmd)
			return ;
		while (cur_tok && cur_tok->type != PIPE)
		{
			if (cur_tok->type == INPUT || cur_tok->type == APPEND
				|| cur_tok->type == OVERWRITE)
				cur_tok = redir_cmd(cur_cmd, cur_tok);
			else if (cur_tok->type == WORD)
				cur_tok = build_cmd(data, cur_cmd, cur_tok);
		}
		ft_lstadd_back_generic((void **)&(data->cmd), cur_cmd, (sizeof(t_cmd) - sizeof(t_cmd *)));
		if (cur_tok)
			cur_tok = cur_tok->next;
	}
}
