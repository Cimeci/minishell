/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:19:29 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/20 17:07:47 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	len = 0;
	res = NULL;
	while (j > 0 && ((str[j] >= 7 && str[j] <= 13) || str[j] == ' '))
		j--;
	while (str[i] && ((str[i] >= 7 && str[i] <= 13) || str[i] == ' '))
		i++;
	if (j == 0)
		return (NULL);
	len = j - i + 1;
	dprintf(2, "\033[1;31m%d|%d|%d\033[0m\n", j, i, len);
	res = malloc(sizeof(char) * len + 1);
	while (k < len)
		res[k++] = str[i++];
	res[k] = '\0';
	return (res);
}
