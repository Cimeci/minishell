/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:38:56 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/17 13:53:46 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parse_heredoc(t_cmd *cur_cmd, t_token *cur_tok)
{
	int	i;

	i = 0;
	cur_cmd->heredoc = malloc((count_token(cur_tok, 1) + 1) * sizeof(char *));
	if (!cur_cmd->heredoc)
		return ;
	while (cur_tok && cur_tok->type != PIPE)
	{
		if (cur_tok->type == HEREDOC)
		{
			cur_cmd->heredoc[i] = ft_strdup(cur_tok->next->str);
			cur_tok->next->type = HEREDOC;
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

void	redir_cmd(t_cmd *cur_cmd, t_token *cur_tok)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cur_tok && cur_tok->type != PIPE)
	{
		if (cur_tok->type == INPUT && cur_tok->next != NULL)
		{
			cur_cmd->infile[i] = ft_strdup(cur_tok->next->str);
			cur_tok->next->type = INPUT;
			cur_tok = cur_tok->next->next;
			i++;
		}
		else if (cur_tok->next != NULL && (cur_tok->type == OVERWRITE
			|| cur_tok->type == APPEND))
		{
			cur_cmd->outfile[j] = ft_strdup(cur_tok->next->str);
			cur_tok->next->type = OVERWRITE;
			cur_tok = cur_tok->next->next;
			j++;
		}
		else if (cur_tok)
			cur_tok = cur_tok->next;
	}
	if (cur_cmd->infile)
		cur_cmd->infile[i] = NULL;
	if (cur_cmd->outfile)
		cur_cmd->outfile[j] = NULL;
}

int	is_built_in(char *str)
{
	if (!ft_strncmp(str, "exit", ft_strlen(str)) && ft_strlen(str) == 4)
		return (1);
	if (!ft_strncmp(str, "cd", ft_strlen(str))
		&& ft_strlen(str) == 2)
		return (1);
	else if (!ft_strncmp(str, "pwd", ft_strlen(str))
		&& ft_strlen(str) == 3)
		return (1);
	else if (!ft_strncmp(str, "echo", ft_strlen(str))
		&& ft_strlen(str) == 4)
		return (1);
	else if (!ft_strncmp(str, "env", ft_strlen(str))
		&& ft_strlen(str) == 3)
		return (1);
	else if (!ft_strncmp(str, "export", ft_strlen(str))
		&& ft_strlen(str) == 6)
		return (1);
	else if (!ft_strncmp(str, "unset", ft_strlen(str))
		&& ft_strlen(str) == 5)
		return (1);
	return (0);
}

t_token	*build_cmd(t_data *data, t_cmd *cur_cmd, t_token *cur_tok)
{
	t_token	*tmp;
	int		i;
	int		len;

	i = 0;
	len = 0;
	tmp = cur_tok;
	if (ft_strchr(cur_tok->str, '/') || cur_tok->type == EMPTY_QUOTE || cur_tok->type == DOT)
		cur_cmd->cmd = ft_strdup(cur_tok->str);
	else
	{
		cur_cmd->cmd = find_path(data, cur_tok->str);
		if (!cur_cmd->cmd)
			cur_cmd->cmd = ft_strdup(cur_tok->str);
	}
	while (tmp && tmp->type != PIPE)
	{
		if (cur_tok->type > 3)
			len++;
		tmp = tmp->next;
	}
	cur_cmd->args = malloc((len + 1) * sizeof(char *));
	if (!cur_cmd->args)
		return (NULL);
	while (cur_tok && cur_tok->type != PIPE)
	{
		if (cur_tok->type <= 3)
			cur_tok = cur_tok->next;
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

int	count_token(t_token *cur, int token)
{
	int		count;

	count = 0;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == token || cur->type == token - 1)
			count++;
		cur = cur->next;
	}
	return (count);
}

void	get_flag_redir(t_data *data, t_cmd *cur_cmd, t_token *cur_tok)
{
	int	count;
	int	i;

	i = 0;
	count = (count_token(cur_tok, 1) + count_token(cur_tok, 3)) / 2;
	if (!count)
		return ;
	cur_cmd->flag_redir = ft_calloc(sizeof(int), count + 1);
	if (!cur_cmd->flag_redir)
		errors(data, NULL, MALLOC);
	while (cur_tok && cur_tok->type != PIPE)
	{
		if (cur_tok->type == OVERWRITE)
		{
			cur_cmd->flag_redir[i] = 1;
			cur_tok = cur_tok->next->next;
			i++;
		}
		else if (cur_tok->type == APPEND)
		{
			cur_cmd->flag_redir[i] = 2;
			cur_tok = cur_tok->next->next;
			i++;
		}
		else if (cur_tok)
			cur_tok = cur_tok->next;
	}
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
			errors(data, NULL, MALLOC);
		if (count_token(cur_tok, 0) > 0)
		{
			cur_cmd->infile = malloc((count_token(cur_tok, 0) + 1) * sizeof(char *));
			if (!cur_cmd->infile)
				errors(data, NULL, MALLOC);
		}
		if (count_token(cur_tok, 3) > 0)
		{
			cur_cmd->outfile = malloc((count_token(cur_tok, 3) + 1) * sizeof(char *));
			if (!cur_cmd->outfile)
				errors(data, NULL, MALLOC);
		}
		redir_cmd(cur_cmd, cur_tok);
		parse_heredoc(cur_cmd, cur_tok);
		get_flag_redir(data, cur_cmd, cur_tok);
		while (cur_tok && cur_tok->type != PIPE)
		{
			if (cur_tok->type <= 3)
				cur_tok = cur_tok->next;
			else if (cur_tok->type >= 4)
				cur_tok = build_cmd(data, cur_cmd, cur_tok);
		}
		ft_lstadd_back_generic((void **)&(data->cmd), cur_cmd, (sizeof(t_cmd) - sizeof(t_cmd *)));
		if (cur_tok)
			cur_tok = cur_tok->next;
	}
}
