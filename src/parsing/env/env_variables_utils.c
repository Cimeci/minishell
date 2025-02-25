/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:17:25 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/25 14:52:44 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*only_dollars(t_data *data, char *line, t_env_var *info)
{
	char	*tmp;
	char	*save;
	int		cur;

	cur = 0;
	while (line[info->i + cur] && line[info->i + cur] == '$'
		&& info->quote_tab[info->dollars + cur] >= 1)
		cur++;
	while (line[info->i] && cur > 0)
	{
		if (cur == 1)
			break ;
		tmp = ft_substr(line, 0, info->i);
		tmp = ft_strjoin_free(tmp, data->shell_pid, 1);
		save = ft_strdup(line);
		free(line);
		line = NULL;
		line = ft_strjoin(tmp, save + info->i + 2);
		free(save);
		info->i = ft_strlen(tmp);
		free(tmp);
		tmp = NULL;
		cur -= 2;
	}
	return (line);
}

int	is_separator_env(char c, int pos)
{
	if (c == '_')
		return (0);
	if (pos == 0 && !ft_isalpha(c))
		return (1);
	if (!ft_isalnum(c))
		return (1);
	return (0);
}

int	count_char(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

char	*before_expand(t_data *data, char *line, t_env_var *info)
{
	line = only_dollars(data, line, info);
	while (line[info->i] && !is_separator_env(line[info->i], 1))
		info->i++;
	return (line);
}
