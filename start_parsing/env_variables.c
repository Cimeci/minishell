/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:32:27 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/17 16:19:01 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	*expansion_quotes(char *line, int nb_var)
{
	int		i;
	int		j;
	int		*quote_tab;
	char	quote;

	i = 0;
	j = 0;
	if (line[0] != '$')
		j = 1;
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


void	env_variables(t_data *data, char *input)
{
	int		i;
	int		j;
	int		*quote_tab;
	char	**table;
	char	*var;
	char	*tmp;

	i = 0;
	j = 0;
	table = ft_split(input, '$');
	while (table[i])
		i++;
	quote_tab = expansion_quotes(input, i);
	i = 0;
	if (input[0] != '$')
		i = 1;
	while (table[i])
	{
		j = 0;
		while (table[i][j] && !IS_SEPARATOR(table[i][j]))
			j++;
		var = ft_substr(table[i], 0, j);
		tmp = ft_substr(table[i], j, ft_strlen(table[i]));
		if (my_getenv(data, var) && quote_tab[i] == 1)
		{
			free(table[i]);
			table[i] = ft_strjoin(ft_strdup(my_getenv(data, var)), tmp);
			free(tmp);
		}
		else if (!my_getenv(data, var) && quote_tab[i] == 1)
		{
			free(table[i]);
			table[i] = tmp;
		}
		else if (quote_tab[i] == 0)
		{
			free(tmp);
			tmp = ft_strdup(table[i]);
			free(table[i]);
			table[i] = ft_strjoin("$", tmp);
			free(tmp);
		}
		free(var);
		i++;
	}
	data->line = ft_strdup("");
	i = 0;
	while (table[i])
		data->line = ft_strjoin_free(data->line, table[i++]);
	ft_free_tab(table);
	free(quote_tab);
}
