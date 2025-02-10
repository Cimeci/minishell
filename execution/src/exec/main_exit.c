/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/10 13:15:09 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

long long	ft_atoll(char *str)
{
	int			i;
	int			sign;
	long long	nb;

	nb = 0;
	sign = 1;
	i = 0;
	if (str[0] == '+' || str[0] == '-')
	{
		if (str[0] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]) && str[i])
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (nb * sign);
}

int	check_max_min(long long nb, char *str)
{
	int			i;
	long long	nb_check;

	nb_check = nb % 10;
	i = ft_strlen(str) - 1;
	if (nb == LLONG_MIN)
	{
		if (ft_strncmp(str, "-9223372036854775808", ft_strlen(str))
			&& ft_strlen(str) == 20)
			return (1);
		return (0);
	}
	if (nb < 0)
		nb = nb * -1;
	while (i > 0)
	{
		if (str[i] != nb_check + 48)
			return (1);
		nb = nb / 10;
		nb_check = nb % 10;
		i--;
	}
	return (0);
}

void	ft_exit(t_data *data, t_cmd *cur)
{
	int			i;
	int			len;
	int			error;
	long long	nb;

	error = 0;
	len = ft_strlen_tab(cur->args) - 1;
	i = 0;
	if (!strncmp(cur->cmd, "exit", ft_strlen(cur->cmd)) && ft_strlen(cur->cmd)
		&& cur->args[1])
	{
		if (len + 1 == 1)
		{
			free_all(data, 0);
			exit(0);
		}
		if (cur->args[1][0] == '-' || cur->args[1][0] == '+')
			i++;
		while (cur->args[1][i])
		{
			if (!ft_isdigit(cur->args[1][i]))
				error = 1;
			i++;
		}
		nb = ft_atoll(cur->args[1]);
		if (error == 0 && !check_max_min(nb, cur->args[1]))
		{
			if (len == 1)
			{
				free_all(data, 0);
				exit(nb % 256);
			}
			printf("exit\nexit : too many arguments\n");
			data->gexit_code = 1;
			return ;
		}
		printf("exit\nbash: exit: %s: numeric argument required\n",
			cur->args[1]);
		free_all(data, 0);
		exit(2);
	}
}
