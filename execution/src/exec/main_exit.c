/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/20 14:42:39 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			check_max_min(long long nb, char *str, int inf);

char	*ft_remove_space(char *str)
{
	int		i;
	int		j;
	int		k;
	int		len;
	char	*res;

	i = 0;
	j = ft_strlen(str) - 1;
	k = 0;
	while (str[j] && ((str[j] >= 7 && str[j] <= 13) || str[j] == ' '))
		j--;
	while (str[i] && ((str[i] >= 7 && str[i] <= 13) || str[i] == ' '))
		i++;
	len = j - i + 1;
	res = malloc(sizeof(char) * len + 1);
	while (k < len)
		res[k++] = str[i++];
	res[k] = '\0';
	return (res);
}

long long	ft_atoll(char *str, int *error)
{
	int			i;
	int			sign;
	long long	nb;
	int			inf;

	inf = 0;
	nb = 0;
	sign = 1;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[0] == '-')
		{
			sign = -1;
			inf = 1;
		}
		i++;
	}
	while (ft_isdigit(str[i]) && str[i])
		nb = nb * 10 + (str[i++] - '0');
	if (!check_max_min(nb, str, inf))
		return (nb * sign);
	*error = 1;
	return (0);
}

int	check_max_min(long long nb, char *str, int inf)
{
	int	i;
	int	j;
	int	nb_check;

	j = 0;
	nb_check = nb % 10;
	i = ft_strlen(str) - 1;
	if (str[0] == '+')
		j++;
	if (inf && str[0] == '-')
		j++;
	while (i >= j)
	{
		if (nb_check < 0 && inf)
			nb_check = nb_check * -1;
		if (str[i] != nb_check + 48)
			return (1);
		nb = nb / 10;
		nb_check = nb % 10;
		i--;
	}
	return (0);
}

void	ft_error_exit(long long nb, t_data *data, t_cmd *cur, int error)
{
	char	*str;

	str = ft_remove_space(cur->args[1]);
	if (error == 1)
	{
		print_errors("exit\nexit: ", str, ": numeric argument required", NULL);
		free(str);
		close_files(cur, data, -1, true);
		free_all(data, 0);
		rl_clear_history();
		exit(2);
	}
	if (ft_strlen_tab(cur->args) - 1 == 1)
	{
		close_files(cur, data, -1, true);
		free_all(data, 0);
		rl_clear_history();
		free(str);

		exit(nb % 256);
	}
	print_errors("exit\nexit :", "too many arguments", NULL);
	data->gexit_code = 1;
	free(str);
}

void	ft_unique_exit(int len, t_data *data, t_cmd *cur)
{
	if (len + 1 == 1)
	{
		close_files(cur, data, -1, true);
		free_all(data, 0);
		rl_clear_history();
		exit(data->gexit_code);
	}
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
		ft_unique_exit(len, data, cur);
		str = ft_remove_space(cur->args[1]);
		if (!str || str[0] == '\0')
		{
			free(str);
			return ;
		}
		if (str[0] == '-' || str[0] == '+')
			i++;
		while (str[i])
		{
			if (!ft_isdigit(str[i]))
				error = 1;
			i++;
		}
		nb = ft_atoll(str, &error);
		free(str);
		ft_error_exit(nb, data, cur, error);
	}
}
