/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/25 10:02:11 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_error_exit(long long nb, t_data *data, t_cmd *cur, int error)
{
	char	*str1;
	char	*str2;

	str1 = ft_remove_space(cur->args[1]);
	str2 = ft_remove_space(cur->args[2]);
	if (error == 1 && (ft_char_is_digit(str2) || ft_char_is_digit(str1)))
	{
		errors_exec("exit\nexit: ", cur->args[1], NUM_ARG);
		close_files(cur, data, -1, true);
		clean_error_exit(data, str1, str2, 1);
		exit(2);
	}
	else if (ft_strlen_tab(cur->args) - 1 != 1 && !ft_char_is_digit(str1))
	{
		clean_error_exit(data, str1, str2, 0);
		errors(data, "exit\nexit: ", ARGS);
		data->gexit_code = 1;
		return ;
	}
	close_files(cur, data, -1, true);
	clean_error_exit(data, str1, str2, 1);
	exit(nb % 256);
}

static void	ft_unique_exit(int len, t_data *data, t_cmd *cur)
{
	if (len + 1 == 1)
	{
		close_files(cur, data, -1, true);
		free_all(data, 0);
		rl_clear_history();
		exit(data->gexit_code);
	}
}

static void	ft_clean_exit(t_data *data, t_cmd *cur)
{
	errors_exec("exit\nexit: ", cur->args[1], NUM_ARG);
	cleanup_execution(data);
	rl_clear_history();
	free_all(data, 0);
	exit(2);
}

static char	*ft_check_exit(t_data *data, t_cmd *cur, int len, int *error)
{
	int		i;
	char	*str;

	i = 0;
	str = NULL;
	ft_unique_exit(len, data, cur);
	if (!cur->args[1] || cur->args[1][0] == '\0')
		ft_clean_exit(data, cur);
	str = ft_remove_space(cur->args[1]);
	if (!str || str[0] == '\0')
	{
		free(str);
		ft_clean_exit(data, cur);
	}
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			*error = 1;
		i++;
	}
	return (str);
}

void	ft_exit(t_data *data, t_cmd *cur)
{
	int			i;
	int			len;
	int			error;
	long long	nb;
	char		*str;

	error = 0;
	len = ft_strlen_tab(cur->args) - 1;
	i = 0;
	if (files(data, cur))
		return ;
	if (!strncmp(cur->cmd, "exit", ft_strlen(cur->cmd)) && ft_strlen(cur->cmd))
	{
		str = ft_check_exit(data, cur, len, &error);
		if (!str)
			return ;
		nb = ft_atoll(str, &error);
		free(str);
		ft_error_exit(nb, data, cur, error);
	}
}
