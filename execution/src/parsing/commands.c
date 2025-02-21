/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:38:56 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/21 12:21:40 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_args(t_cmd *cur_cmd, t_token *cur_tok)
{
	int	i;

	i = 0;
	while (cur_tok && cur_tok->type != PIPE)
	{
		if (cur_tok->type <= 3)
		{
			if (cur_tok->next == NULL)
				cur_tok = cur_tok->next;
			else if (cur_tok->next->type <= PIPE)
				break ;
			else
				cur_tok = cur_tok->next->next;
		}
		else if (cur_tok->empty_var_tok == false)
		{
			cur_cmd->args[i] = ft_strdup(cur_tok->str);
			i++;
			cur_tok = cur_tok->next;
		}
		else if (cur_tok)
			cur_tok = cur_tok->next;
	}
	cur_cmd->args[i] = NULL;
	return (cur_tok);
}

void	save_cmd(t_data *data, t_cmd *cur_cmd, t_token *cur_tok)
{
	char	*get_env;

	get_env = NULL;
	if (ft_strchr(cur_tok->str, '/') || cur_tok->str[0] == '\0'
		|| cur_tok->type == DOT)
		cur_cmd->cmd = ft_strdup(cur_tok->str);
	else
	{
		cur_cmd->cmd = find_path(data, cur_tok->str);
		get_env = my_getenv_lst("PATH", data->env);
		if (!cur_cmd->cmd && !get_env && !is_built_in(cur_tok->str))
			cur_cmd->cmd = ft_strjoin("./", cur_tok->str);
		else if (!cur_cmd->cmd)
			cur_cmd->cmd = ft_strdup(cur_tok->str);
		free(get_env);
	}
}

t_token	*build_cmd(t_data *data, t_cmd *cur_cmd, t_token *cur_tok)
{
	t_token	*tmp;
	int		i;
	int		len;

	i = 0;
	len = 0;
	tmp = cur_tok;
	cur_cmd->empty_var_cmd = false;
	while (cur_tok && cur_tok->empty_var_tok == true)
	{
		cur_tok = cur_tok->next;
		if (cur_tok == NULL)
		{
			cur_cmd->empty_var_cmd = true;
			return (cur_tok);
		}
	}
	save_cmd(data, cur_cmd, cur_tok);
	len = count_args(cur_tok);
	cur_cmd->args = malloc((len + 1) * sizeof(char *));
	if (!cur_cmd->args)
		errors(data, NULL, MALLOC);
	cur_tok = get_args(cur_cmd, cur_tok);
	return (cur_tok);
}

t_token	*find_cmd(t_data *data, t_token *cur_tok, t_cmd *cur_cmd)
{
	if (cur_tok->type <= 3)
	{
		cur_tok = cur_tok->next;
		if (cur_tok)
			cur_tok = cur_tok->next;
	}
	else if (cur_tok->type >= 4)
		cur_tok = build_cmd(data, cur_cmd, cur_tok);
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
			errors(data, NULL, MALLOC);
		manage_redirs(data, cur_cmd, cur_tok);
		while (cur_tok && cur_tok->type != PIPE)
			cur_tok = find_cmd(data, cur_tok, cur_cmd);
		ft_lstadd_back_generic((void **)&(data->cmd), cur_cmd, (sizeof(t_cmd)
				- sizeof(t_cmd *)));
		if (cur_tok)
			cur_tok = cur_tok->next;
	}
}
