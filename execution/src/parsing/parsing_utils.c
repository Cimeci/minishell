/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:40:02 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/22 09:31:13 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	*ft_lstnew_generic(size_t data_size)
{
	void	*new_node;

	new_node = malloc(data_size);
	if (!new_node)
		return (NULL);
	memset(new_node, 0, data_size);
	return (new_node);
}

void ft_lstadd_back_generic(void **lst, void *new_node, size_t next_offset)
{
	void	*temp;

	if (!lst || !new_node)
		return;
	temp = *lst;
	if (*lst == NULL)
	{
		*lst = new_node;
		*(void **)((char *)new_node + next_offset) = NULL;
		return ;
	}
	while (*(void **)((char *)temp + next_offset) != NULL)
		temp = *(void **)((char *)temp + next_offset);
	if (temp)
		*(void **)((char *)temp + next_offset) = new_node;
}

int	ft_lstsize_generic(void *lst, size_t offset)
{
	int	i;

	i = 0;
	if (!lst)
		return (0);
	if ((void *)((char *)lst + offset) == NULL)
		return (1);
	while ((void *)((char *)lst + offset) != NULL)
	{
		lst = (void *)((char *)lst + offset);
		i++;
	}
	return (i + 1);
}

int	check_quotes(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
		{
			i++;
			while (input[i] && input[i] != '\'')
				i++;
			if (input[i] != '\'')
				return (-1);
		}
		if (input[i] == '"')
		{
			i++;
			while (input[i] && input[i] != '"')
				i++;
			if (input[i] != '"')
				return (-1);
		}
		i++;
	}
	return (0);
}

void	get_shell_pid(t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return ;
	if (pid == 0)
		exit(0);
	else
	{
		data->shell_pid = ft_itoa(pid);
		if (!data->shell_pid)
		{
			perror("ft_itoa failed");
			exit(1);
		}
	}
	wait(NULL);
	return ;
}
