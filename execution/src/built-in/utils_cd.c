/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:50:18 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/21 14:25:38 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free_path(char *path, char *target_path)
{
	free(path);
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
