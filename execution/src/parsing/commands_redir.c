/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:47:04 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/21 11:54:34 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_heredoc(t_cmd *cur_cmd, t_token *cur_tok)
{
	int	i;

	i = 0;
	cur_cmd->here = 0;
	cur_cmd->heredoc = malloc((count_token(cur_tok, 1) + 1) * sizeof(char *));
	if (!cur_cmd->heredoc)
		return ;
	while (cur_tok && cur_tok->type != PIPE)
	{
		if (cur_tok->type == HEREDOC && cur_tok->next != NULL)
		{
			cur_cmd->heredoc[i] = ft_strdup(cur_tok->next->str);
			if (cur_tok->next->expand == true)
				cur_cmd->expand = true;
			else
				cur_cmd->expand = false;
			cur_cmd->here = 1;
			cur_tok = cur_tok->next->next;
			i++;
		}
		else if (cur_tok)
			cur_tok = cur_tok->next;
	}
	if (cur_cmd->heredoc)
		cur_cmd->heredoc[i] = NULL;
}

void	save_files(t_cmd *cur_cmd, t_token *cur_tok, int i, int j)
{
	while (cur_tok && cur_tok->type != PIPE)
	{
		if (cur_tok->type == INPUT && cur_tok->next != NULL)
		{
			cur_cmd->infile[i] = ft_strdup(cur_tok->next->str);
			i++;
			if (access(cur_tok->next->str, R_OK) != 0)
				return ;
			cur_tok = cur_tok->next->next;
		}
		else if (cur_tok->next != NULL && (cur_tok->type == OVERWRITE
				|| cur_tok->type == APPEND))
		{
			cur_cmd->outfile[j] = ft_strdup(cur_tok->next->str);
			j++;
			if (access(cur_tok->next->str, F_OK) == 0
				&& access(cur_tok->next->str, W_OK) != 0)
				return ;
			cur_tok = cur_tok->next->next;
		}
		else if (cur_tok)
			cur_tok = cur_tok->next;
	}
}

void	redir_cmd(t_cmd *cur_cmd, t_token *cur_tok)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	save_files(cur_cmd, cur_tok, i, j);
	if (cur_cmd->infile)
		cur_cmd->infile[i] = NULL;
	if (cur_cmd->outfile)
		cur_cmd->outfile[j] = NULL;
}

void	get_flag_redir(t_data *data, t_cmd *cur_cmd, t_token *cur_tok)
{
	int	count;
	int	i;

	i = 0;
	count = (count_token(cur_tok, 1) + count_token(cur_tok, 3));
	if (!count)
		return ;
	cur_cmd->flag_redir = ft_calloc(sizeof(int), count + 1);
	if (!cur_cmd->flag_redir)
		errors(data, NULL, MALLOC);
	while (cur_tok && cur_tok->type != PIPE && cur_tok->next != NULL)
	{
		if (cur_tok->type == OVERWRITE || cur_tok->type == APPEND)
		{
			if (cur_tok->type == OVERWRITE)
				cur_cmd->flag_redir[i] = 1;
			else if (cur_tok->type == APPEND)
				cur_cmd->flag_redir[i] = 2;
			cur_tok = cur_tok->next->next;
			i++;
		}
		else if (cur_tok)
			cur_tok = cur_tok->next;
	}
}

void	manage_redirs(t_data *data, t_cmd *cur_cmd, t_token *cur_tok)
{
	if (count_token(cur_tok, 0) > 0)
	{
		cur_cmd->infile = malloc((count_token(cur_tok, 0) + 1)
				* sizeof(char *));
		if (!cur_cmd->infile)
			errors(data, NULL, MALLOC);
	}
	if (count_token(cur_tok, 3) > 0)
	{
		cur_cmd->outfile = malloc((count_token(cur_tok, 3) + 1)
				* sizeof(char *));
		if (!cur_cmd->outfile)
			errors(data, NULL, MALLOC);
	}
	redir_cmd(cur_cmd, cur_tok);
	parse_heredoc(cur_cmd, cur_tok);
	get_flag_redir(data, cur_cmd, cur_tok);
}
