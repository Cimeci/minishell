/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 17:08:00 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/21 12:24:03 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rebuild_cmd(t_data *data, char *str)
{
	t_token	*new;
	int		i;
	int		start;

	i = 0;
	start = 0;
	while (str[i])
	{
		while (str[i] && is_white_space(str[i]))
			i++;
		start = i;
		while (str[i] && !is_white_space(str[i]))
			i++;
		new = (t_token *)ft_lstnew_generic(sizeof(t_token));
		if (!new)
			errors(data, NULL, MALLOC);
		new->str = ft_substr(str, start, i - start);
		new->type = WORD;
		new->empty_var_tok = false;
		ft_lstadd_back_generic((void **)&data->token, new, (sizeof(t_token)
				- sizeof(t_token *)));
	}
	free(str);
}

int	skip_in_quotes(char *str, int i)
{
	char	quote;
	int		start;

	quote = str[i];
	i++;
	start = i;
	while (str[i] && str[i] != quote)
		i++;
	return (i);
}

char	*remove_quotes(t_data *data, char *str)
{
	char	*dest;
	int		i;
	int		start;

	i = 0;
	start = 0;
	dest = malloc(1);
	if (!dest)
		errors(data, NULL, MALLOC);
	dest[0] = '\0';
	while (str[i])
	{
		start = i;
		while (str[i] && (str[i] != SINGLE_QUOTE && str[i] != DOUBLE_QUOTE))
			i++;
		dest = ft_strjoin_free(dest, ft_substr(str, start, i - start), 3);
		if (str[i] && (str[i] == SINGLE_QUOTE || str[i] == DOUBLE_QUOTE))
		{
			start = i + 1;
			i = skip_in_quotes(str, i);
			dest = ft_strjoin_free(dest, ft_substr(str, start, i - start), 3);
			i++;
		}
	}
	return (dest);
}

int	count_words(char *str)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (str[i])
	{
		while (str[i] && str[i] == ' ')
			i++;
		if (str[i] && str[i] != ' ')
			words++;
		while (str[i] && str[i] != ' ')
			i++;
	}
	free(str);
	return (words);
}

int	is_separator_token(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == ' ')
		return (1);
	else if (c >= 7 && c <= 13)
		return (1);
	return (0);
}
