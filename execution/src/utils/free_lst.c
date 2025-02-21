/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:16:33 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/21 15:22:16 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
