/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:32:27 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/24 15:00:03 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	*expansion_quotes(char *line, int nb_var)
{
	int		i;
	int		j;
	int		*quote_tab;
	char	quote;

	i = 0;
	j = 0;
	quote_tab = malloc(nb_var * sizeof(int));
	if (!quote_tab)
		return (0);
	while (line[i])
	{
		if (IS_QUOTE(line[i]))
		{
			quote = line[i];
			i++;
			while (line[i] && line[i] != quote)
			{
				if (line[i] == '$' && quote == SINGLE_QUOTE)
					quote_tab[j++] = 0;
				if (line[i] == '$' && quote == DOUBLE_QUOTE)
					quote_tab[j++] = 1;
				i++;
			}
		}
		if (line[i] && line[i] == '$')
			quote_tab[j++] = 1;
		i++;
	}
	return (quote_tab);
}

int	only_dollars(t_data *data, int *quote_tab, int dollars, int i)
{
	char	*tmp;
	char	*save;
	int		cur;
	int		index;

	cur = 0;
	while (data->line[i + cur] && data->line[i + cur] == '$' && quote_tab[dollars + cur] == 1)
		cur++;
	index = cur;
	while (data->line[i] && cur > 0)
	{
		if (cur == 1)
			break ;
		tmp = ft_substr(data->line, 0, i);
		tmp = ft_strjoin_free(tmp, data->shell_pid);
		save = ft_strdup(data->line);
		free(data->line);
		data->line = NULL;
		data->line = ft_strjoin(tmp, save + i + 2);
		i = ft_strlen(tmp);
		free(tmp);
		tmp = NULL;
		cur -= 2;
	}
	return (index + dollars);
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

void	env_variables(t_data *data)
{
	int		i;
	int		result;
	int		dollars;
	int		*quote_tab;
	char	*var;
	char	*next;
	char	*prev;

	i = 0;
	dollars = 0;
	result = 0;
	while (data->line[i])
	{
		if (data->line[i] == '$')
			dollars++;
		i++;
	}
	quote_tab = expansion_quotes(data->line, dollars);
	dollars = 0;
	i = 0;
	while (data->line[i])
	{
		if (data->line[i] == '$')
		{
			result = only_dollars(data, quote_tab, dollars, i);
			while (data->line[i] && !is_separator_env(data->line[i], 1))
				i++;
			dollars = result - (result % 2);
			if (data->line[i] != '$')
				;
			else
			{
				i++;
				result = 0;
				while (data->line[i + result] && !is_separator_env(data->line[i + result], result))
					result++;
				if (data->line[i + result] == '_')
					result--;
				var = ft_substr(data->line, i, result);
				next = ft_substr(data->line, i + result, ft_strlen(data->line));
				prev = ft_substr(data->line, 0, i - 1);
				free(data->line);
				if (result == 0 && var[0] == '\0')
				{
					prev = ft_strjoin_free(prev, "$");
					data->line = ft_strjoin(prev, next);
				}
				else if (my_getenv(data, var) && quote_tab[dollars] == 1)
				{
					prev = ft_strjoin_free(prev, my_getenv(data, var));
					data->line = ft_strjoin(prev, next);
				}
				else if (!my_getenv(data, var) && quote_tab[dollars] == 1)
					data->line = ft_strjoin(prev, next);
				else if (quote_tab[dollars] == 0)
				{
					prev = ft_strjoin_free(prev, "$");
					prev = ft_strjoin_free(prev, var);
					data->line = ft_strjoin(prev, next);
				}
				free(next);
				free(prev);
				dollars++;
			}
		}
		else
			i++;
	}
	free(quote_tab);
}
