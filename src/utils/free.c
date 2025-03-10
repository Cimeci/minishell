/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:46:10 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/27 10:44:32 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_pid(t_list **head)
{
	t_list	*current;
	t_list	*next_node;

	current = *head;
	next_node = NULL;
	while (current)
	{
		next_node = current->next;
		free(current->content);
		current->content = NULL;
		free(current);
		current = next_node;
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
	if (data->pid)
		free_pid(&data->pid);
	data->line = NULL;
	data->token = NULL;
	data->cmd = NULL;
	data->env_cp = NULL;
}

char	*ft_strjoin_free(char *s1, char *s2, int flag)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	if (flag == 1)
		free(s1);
	else if (flag == 2)
		free(s2);
	else if (flag == 3)
	{
		free(s1);
		free(s2);
	}
	return (tmp);
}
