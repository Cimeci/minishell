/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:50:18 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 09:52:11 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_path(char *path, char *target_path)
{
	free(path);
	free(target_path);
}

void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	while (table[i])
		free(table[i++]);
	free(table);
	table = NULL;
}

char	*ft_strsstr(char *pwd, char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] == pwd[i])
		i++;
	if (!cmd[i] || !pwd[i])
		return (cmd);
	return (NULL);
}
