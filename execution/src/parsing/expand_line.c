/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:15:29 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/21 11:36:06 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*no_var(t_data *data, char *line, t_env_var *info)
{
	if (info->next[0] == '?')
	{
		if (g_exit_code_sig)
		{
			info->prev = ft_strjoin_free(info->prev,
					ft_itoa(g_exit_code_sig), 3);
			g_exit_code_sig = 0;
		}
		else
			info->prev = ft_strjoin_free(info->prev,
					ft_itoa(data->gexit_code), 3);
		line = ft_strjoin(info->prev, info->next + 1);
	}
	else
	{
		if (info->quote_tab[info->dollars] == 1
			|| ((info->next[0] != '"' && info->next[0] != '\'')
				&& info->quote_tab[info->dollars] == 2))
			info->prev = ft_strjoin_free(info->prev, "$", 1);
		line = ft_strjoin(info->prev, info->next);
	}
	return (line);
}

char	*var_ok(t_data *data, char *line, t_env_var *info)
{
	if (info->heredoc == false && info->quote_tab[info->dollars] == 2
		&& count_words(ft_strjoin(info->prev, my_getenv(data, info->var))) > 1)
	{
		rebuild_cmd(data, ft_strjoin(info->prev, my_getenv(data, info->var)));
		line = ft_strdup(info->next);
		if (!line || line[0] == '\0')
		{
			free(line);
			free(info->prev);
			free(info->var);
			free(info->next);
			free(info->quote_tab);
			return (NULL);
		}
	}
	else
	{
		info->prev = ft_strjoin_free(info->prev, my_getenv(data, info->var), 1);
		line = ft_strjoin(info->prev, info->next);
	}
	return (line);
}

char	*quote_not_ok(char *line, t_env_var *info)
{
	info->prev = ft_strjoin_free(info->prev, "$", 1);
	info->prev = ft_strjoin_free(info->prev, info->var, 1);
	line = ft_strjoin(info->prev, info->next);
	return (line);
}

char	*expand_line(t_data *data, char *line, t_env_var *info)
{
	int		result;

	result = 0;
	while (line[info->i + result]
		&& !is_separator_env(line[info->i + result], result))
		result++;
	if (line[info->i + result] == '_')
		result--;
	info->var = ft_substr(line, info->i, result);
	info->next = ft_substr(line, info->i + result, ft_strlen(line));
	info->prev = ft_substr(line, 0, info->i - 1);
	free(line);
	if (result == 0 && info->var[0] == '\0'
		&& info->quote_tab[info->dollars] >= 1)
		line = no_var(data, line, info);
	else if (my_getenv(data, info->var) && info->quote_tab[info->dollars] >= 1)
		line = var_ok(data, line, info);
	else if (!my_getenv(data, info->var) && info->quote_tab[info->dollars] >= 1)
		line = ft_strjoin(info->prev, info->next);
	else if (info->quote_tab[info->dollars] == 0)
		line = quote_not_ok(line, info);
	return (line);
}

char	*is_var(t_data *data, char *line, t_env_var *info)
{
	info->i++;
	if (!line[info->i])
		return (NULL);
	line = expand_line(data, line, info);
	if (!line || line[0] == '\0')
		return (NULL);
	info->i = ft_strlen(info->prev);
	info->dollars++;
	free(info->next);
	free(info->prev);
	free(info->var);
	return (line);
}
