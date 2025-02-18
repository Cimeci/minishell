/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:46:10 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/17 16:28:09 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	if (!table)
		return ;
	while (table[i])
		free(table[i++]);
	free(table);
	table = NULL;
}

void	free_token(t_token **head)
{
	t_token	*current;
	t_token	*next_node;

	current = *head;
	next_node = NULL;
	while (current)
	{
		next_node = current->next;
		free(current->str);
		current->str = NULL;
		free(current);
		current = next_node;
	}
	*head = NULL;
}

void	free_env(t_lst **head)
{
	t_lst	*current;
	t_lst	*next_node;

	current = *head;
	next_node = NULL;
	// dprintf(2, "|free env|");
	while (current)
	{
		next_node = current->next;
		free(current->str);
		current->str = NULL;
		free(current);
		current = next_node;
	}
	*head = NULL;
}

void	free_cmd(t_cmd **head)
{
	t_cmd	*cur;
	t_cmd	*next_node;

	cur = *head;
	next_node = NULL;
	while (cur)
	{
		next_node = cur->next;
		free(cur->cmd);
		cur->cmd = NULL;
		ft_free_tab(cur->args);
		ft_free_tab(cur->outfile);
		ft_free_tab(cur->infile);
		ft_free_tab(cur->heredoc);
		free(cur->flag_redir);
		cur->flag_redir = 0;
		if (cur->file)
		{
			unlink(cur->file);
			free(cur->file);
			cur->file = NULL;
		}
		free(cur);
		cur = next_node;
	}
	*head = NULL;
}

void	free_all(t_data *data, int flag)
{
	if (data->token)
		free_token(&data->token);
	if (data->env && flag == 0)
		free_env(&data->env);
	if (data->shell_pid && flag == 0)
		free(data->shell_pid);
	if (data->line)
		free(data->line);
	if (data->cmd)
		free_cmd(&data->cmd);
	if (data->pwd)
		free(data->pwd);
	if (data->env_cp)
		ft_free_tab(data->env_cp);
	data->line = NULL;
	data->token = NULL;
	data->cmd = NULL;
	data->env_cp = NULL;
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	free(s1);
	return (tmp);
}
