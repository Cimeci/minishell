/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:25:10 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 11:39:04 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	if (table)
	{
		while (table[i])
		{
			if (table[i])
				free(table[i]);
			i++;
		}
		free(table);
		table = NULL;
	}
}
