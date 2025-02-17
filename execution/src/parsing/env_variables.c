/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:32:27 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/17 10:31:41 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	*expansion_quotes(char *line, int nb_var, bool heredoc)
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
				if (line[i] == '$' && (quote == SINGLE_QUOTE && heredoc == false))
					quote_tab[j++] = 0;
				if (line[i] == '$' && (quote == DOUBLE_QUOTE || heredoc == true))
					quote_tab[j++] = 1;
				i++;
			}
		}
		if (line[i] && line[i] == '$')
			quote_tab[j++] = 2;
		i++;
	}
	return (quote_tab);
}

char	*only_dollars(t_data *data, char *line, int *quote_tab, int dollars, int i)
{
	char	*tmp;
	char	*save;
	int		cur;

	cur = 0;
	while (line[i + cur] && line[i + cur] == '$' && quote_tab[dollars + cur] == 1)
		cur++;
	while (line[i] && cur > 0)
	{
		if (cur == 1)
			break ;
		tmp = ft_substr(line, 0, i);
		tmp = ft_strjoin_free(tmp, data->shell_pid);
		save = ft_strdup(line);
		free(line);
		line = NULL;
		line = ft_strjoin(tmp, save + i + 2);
		i = ft_strlen(tmp);
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

void	add_env_token(t_data *data, char *line, int flag_expand)
{
	t_token	*new;

	new = (t_token *)ft_lstnew_generic(sizeof(t_token));
	if (!new)
		errors(data, NULL, MALLOC);
	new->str = line;
	new->type = WORD;
	new->expand = flag_expand;
	ft_lstadd_back_generic((void **)&data->token, new, (sizeof(t_token) - sizeof(t_token *)));
}

char *env_variables(t_data *data, char *line, bool heredoc)
{
	int		i;
	int		result;
	int		dollars;
	int		*quote_tab;
	char	*var;
	char	*next;
	char	*prev;

	i = 0;
	dollars = count_char(line, '$');
	result = 0;
	quote_tab = expansion_quotes(line, dollars, heredoc);
	if (heredoc == false)
		line = remove_quotes(ft_strdup(line));
	dollars = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			line = only_dollars(data, line, quote_tab, dollars, i);
			while (line[i] && !is_separator_env(line[i], 1))
				i++;
			if (line[i] == '$')
			{
				i++;
				result = 0;
				while (line[i + result] && !is_separator_env(line[i + result], result))
					result++;
				if (line[i + result] == '_')
					result--;
				var = ft_substr(line, i, result);
				next = ft_substr(line, i + result, ft_strlen(line));
				prev = ft_substr(line, 0, i - 1);
				free(line);
				if (result == 0 && var[0] == '\0' && quote_tab[dollars] >= 1)
				{
					if (next[0] == '?')
					{
						if (g_exit_code_sig)
							prev = ft_strjoin_free(prev, ft_itoa(g_exit_code_sig));
						else
							prev = ft_strjoin_free(prev, ft_itoa(data->gexit_code));
						line = ft_strjoin(prev, next + 1);
					}
					else
					{
						prev = ft_strjoin_free(prev, "$");
						line = ft_strjoin(prev, next);
					}
				}
				else if (my_getenv(data, var) && quote_tab[dollars] >= 1)
				{
					if (heredoc == false && quote_tab[dollars] == 2 && count_words(ft_strjoin(prev, my_getenv(data, var))) > 1)
					{
						rebuild_cmd(data, ft_strjoin(prev, my_getenv(data, var)));
						line = ft_strdup(next);
						if (!line || line[0] == '\0')
						{
							free(line);
							return (NULL);
						}
					}
					else
					{
						prev = ft_strjoin_free(prev, my_getenv(data, var));
						line = ft_strjoin(prev, next);
					}
				}
				else if (!my_getenv(data, var) && quote_tab[dollars] >= 1)
					line = ft_strjoin(prev, next);
				else if (quote_tab[dollars] == 0)
				{
					prev = ft_strjoin_free(prev, "$");
					prev = ft_strjoin_free(prev, var);
					line = ft_strjoin(prev, next);
				}
				free(next);
				free(prev);
				free(var);
				dollars++;
			}
		}
		else
			i++;
	}
	free(quote_tab);
	if (heredoc == true)
		return (line);
	else
		add_env_token(data, line, true);
	return (NULL);
}
