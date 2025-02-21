/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:48:50 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/20 18:49:42 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_built_in(char *str)
{
	if (!ft_strncmp(str, "exit", ft_strlen(str)) && ft_strlen(str) == 4)
		return (1);
	if (!ft_strncmp(str, "cd", ft_strlen(str)) && ft_strlen(str) == 2)
		return (1);
	else if (!ft_strncmp(str, "pwd", ft_strlen(str)) && ft_strlen(str) == 3)
		return (1);
	else if (!ft_strncmp(str, "echo", ft_strlen(str)) && ft_strlen(str) == 4)
		return (1);
	else if (!ft_strncmp(str, "env", ft_strlen(str)) && ft_strlen(str) == 3)
		return (1);
	else if (!ft_strncmp(str, "export", ft_strlen(str)) && ft_strlen(str) == 6)
		return (1);
	else if (!ft_strncmp(str, "unset", ft_strlen(str)) && ft_strlen(str) == 5)
		return (1);
	return (0);
}

int	count_token(t_token *cur, int token)
{
	int	count;

	count = 0;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == token || cur->type == token - 1)
			count++;
		cur = cur->next;
	}
	return (count);
}

int	count_args(t_token *tmp)
{
	int	len;

	len = 0;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type > 3 && tmp->empty_var_tok == false)
			len++;
		tmp = tmp->next;
	}
	return (len);
}
