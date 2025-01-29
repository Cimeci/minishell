/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:33:03 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/29 10:37:17 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	errors(t_data *data, char *str, int type)
{
	if (type == ERROR_SYNTAX)
	{
		printf("syntax error near unexpected token '%s'\n", str);
		data->gexit_code = 2;
	}
	else if (type == CMD_NOT_FOUND)
		printf("%s: command not found\n", str);
	else if (type == FILES)
		printf("%s: no such file or directory\n", str);
	else if (type == PERM)
		printf("%s: permission denied\n", str);
	else if (type == DIRECTORY)
	{
		printf("%s: is a directory\n", str);
		data->gexit_code = 126;
	}
	return (0);
}
