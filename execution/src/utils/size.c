/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   size.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:30:09 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/06 11:30:07 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strlen_tab(char **table)
{
	int	i;

	i = 0;
	while (table[i])
		i++;
	return (i);
}
