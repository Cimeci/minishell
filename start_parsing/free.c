/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 17:12:16 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/09 14:36:15 by ncharbog         ###   ########.fr       */
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

void	free_all(t_data *data)
{
	if (data->token)
		free_token(&data->token);
	if (data->env)
		free_env(&data->env);
	data->token = NULL;
	data->env = NULL;
}
