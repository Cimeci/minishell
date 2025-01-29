/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:50:18 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/28 17:56:14 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free_path(char *path, char *target_path)
{
	if (path)
		free(path);
	if (target_path)
		free(target_path);
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
