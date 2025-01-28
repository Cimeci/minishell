/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:33:03 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/28 16:14:27 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	errors(t_data *data, char *str, int type)
{
	if (type == ERROR_SYNTAX)
	{
		printf("bash: syntax error near unexpected token '%s'\n", str);
		data->gexit_code = 2;
	}
	else if (type == CMD_NOT_FOUND)
	{
		printf("%s: command not found", str);
		data->gexit_code = 127;
	}
	return (0);
}
