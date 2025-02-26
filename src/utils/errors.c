/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:33:03 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/26 17:19:17 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_errors(char *str, ...)
{
	char	*arg;
	va_list	args;

	if (!str)
		return ;
	va_start(args, str);
	arg = va_arg(args, char *);
	str = ft_strjoin(str, arg);
	arg = va_arg(args, char *);
	while (arg)
	{
		str = ft_strjoin_free(str, arg, 1);
		arg = va_arg(args, char *);
	}
	va_end(args);
	ft_putendl_fd(str, 2);
	free(str);
}

int	errors(t_data *data, char *str, int type)
{
	if (type == ERROR_SYNTAX)
	{
		print_errors("syntax error near unexpected token '", str, "'", NULL);
		data->gexit_code = 2;
	}
	else if (type == CMD_NOT_FOUND)
		print_errors(str, ": command not found", NULL);
	else if (type == FILES)
		print_errors(str, ": No such file or directory", NULL);
	else if (type == PERM)
		print_errors(str, ": Permission denied", NULL);
	else if (type == DIRECTORY)
		print_errors(str, ": Is a directory", NULL);
	else if (type == ARGS)
		print_errors(str, ": too many arguments", NULL);
	else if (type == MALLOC)
	{
		print_errors("Malloc error", NULL);
		free_all(data, 0);
		exit(EXIT_FAILURE);
	}
	else if (type == ERRNO)
		print_errors(str, ": ", strerror(errno), NULL);
	return (0);
}

void	errors_exec(char *msg, char *cmd, int type)
{
	if (type == FILES)
		print_errors(cmd, ": ", msg, ": No such file or directory", NULL);
	if (type == IDENTIFIER)
		print_errors(cmd, ": '", msg, "': not a valid identifier", NULL);
	if (type == NUM_ARG)
		print_errors(msg, cmd, ": numeric argument required", NULL);
	if (type == NOT_DIR)
		print_errors(cmd, ": ", msg, ": Not a directory", NULL);
}
