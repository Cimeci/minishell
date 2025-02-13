/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:17:41 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/13 15:21:23 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_get_var(char *str)
{
	char	*var;
	int		i;

	i = 0;
	while (str[i] != '=' && str[i])
		i++;
	if (!str[i])
		return (NULL);
	var = ft_substr(str, 0, i);
	return (var);
}

char	*ft_get_pvar(char *argv)
{
	int	i;

	i = 0;
	while (argv[i] != '+' && argv[i])
		i++;
	if (!argv[i])
		return (NULL);
	i++;
	if (argv[i] == '=')
		return (ft_substr(argv, 0, i - 1));
	return (NULL);
}

char	*ft_get_var_and_value(char *var, t_lst *env)
{
	int		j;
	char	*tmp;
	char	*tmp_dup;
	t_lst	*cur;

	cur = env;
	while (cur)
	{
		j = 0;
		while (cur->str[j] != '=' && cur->str[j])
			j++;
		tmp_dup = ft_strdup(cur->str);
		tmp = ft_substr(tmp_dup, 0, j);
		if (!ft_strncmp(tmp, var, ft_strlen(var)))
		{
			free(tmp_dup);
			free(tmp);
			return (cur->str);
		}
		free(tmp_dup);
		free(tmp);
		cur = cur->next;
	}
	return (NULL);
}

char	*ft_get_value(char *argv)
{
	int	i;

	i = 0;
	while (argv[i] != '=' && argv[i])
		i++;
	if (argv[i] == '=')
		return (ft_substr(argv, i + 1, ft_strlen(argv) - i));
	return (NULL);
}
