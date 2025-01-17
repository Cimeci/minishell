/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 17:12:16 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/20 13:22:14 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
		free(cur->outfile);
		cur->outfile = NULL;
		free(cur->infile);
		cur->infile = NULL;
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
	if (data->line)
		free(data->line);
	if (data->cmd)
		free_cmd(&data->cmd);
	data->line = NULL;
	data->token = NULL;
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	free(s1);
	return (tmp);
}
