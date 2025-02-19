/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:34:49 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/19 10:47:34 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *str)
{
	char	*dest;
	char	quote;
	int		i;
	int		start;
	int		len;

	i = 0;
	len = 0;
	start = 0;
	dest = malloc(1);
	dest[0] = '\0';
	while (str[i])
	{
		start = i;
		while (str[i + len] && !IS_QUOTE(str[i + len]))
			len++;
		dest = ft_strjoin_free(dest, ft_substr(str, start, len), 3);
		i += len;
		len = 0;
		if (str[i] && IS_QUOTE(str[i]))
		{
			quote = str[i];
			i++;
			while (str[i + len] && str[i + len] != quote)
				len++;
			dest = ft_strjoin_free(dest, ft_substr(str, i, len), 3);
			i += len + 1;
		}
		len = 0;
	}
	free(str);
	return (dest);
}

int	get_token_len(char *str)
{
	int		i;
	char	quote;

	i = 0;
	while (str[i])
	{
		if (IS_QUOTE(str[i]))
		{
			quote = str[i];
			i++;
			while (str[i] != quote)
				i++;
		}
		if (IS_SEPARATOR_TOKEN(str[i]))
		{
			if (i == 0)
				i++;
			if ((str[0] == '>' && str[i] == '>') || (str[0] == '<'
					&& str[i] == '<'))
				i++;
			break ;
		}
		i++;
	}
	return (i);
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

void	rebuild_cmd(t_data *data, char *str)
{
	t_token	*new;
	int		i;
	int		start;
	int		len;

	i = 0;
	start = 0;
	len = 0;
	while (str[i])
	{
		while (str[i] && str[i] == ' ')
			i++;
		start = i;
		len = 0;
		while (str[i + len] && str[i + len] != ' ')
			len++;
		new = (t_token *)ft_lstnew_generic(sizeof(t_token));
		if (!new)
			errors(data, NULL, MALLOC);
		new->str = ft_substr(str, start, len);
		new->type = WORD;
		new->empty_var_tok = false;
		ft_lstadd_back_generic((void **)&data->token, new, (sizeof(t_token)
				- sizeof(t_token *)));
		i += len;
	}
	free(str);
}

void	add_token(t_data *data, int i)
{
	t_token	*last;
	t_token	*cur;
	int		len;
	char	*str;

	last = ft_lstlast_generic(data->token, (sizeof(t_token)
				- sizeof(t_token *)));
	len = get_token_len(data->line + i);
	str = ft_substr(data->line, i, len);
	if (ft_strchr(str, '$') && (!last || last->type != HEREDOC))
		env_variables(data, str, false);
	else
	{
		cur = (t_token *)ft_lstnew_generic(sizeof(t_token));
		if (!cur)
			errors(data, NULL, MALLOC);
		cur->expand = true;
		if ((ft_strnstr(str, "\"", len) || ft_strnstr(str, "'", len)) && last
					&& last->type == HEREDOC)
			cur->expand = false;
		get_token(str, cur);
		cur->str = remove_quotes(str);
		cur->empty_var_tok = false;
		if (ft_strlen(cur->str) == 1 && !ft_strncmp(cur->str, ".",
				ft_strlen(cur->str)))
			cur->type = DOT;
		if (ft_strlen(cur->str) == 2 && !ft_strncmp(cur->str, "..",
				ft_strlen(cur->str)))
			cur->type = DOT;
		ft_lstadd_back_generic((void **)&data->token, cur, (sizeof(t_token)
				- sizeof(t_token *)));
	}
}

void	get_token(char *str, t_token *cur)
{
	int		len;
	char	*only_token;

	len = get_token_len(str);
	only_token = ft_substr(str, 0, len);
	if (ft_strlen(only_token) == 1 && !ft_strncmp(str, "|", len))
		cur->type = PIPE;
	else if (ft_strlen(only_token) == 2 && !ft_strncmp(str, "<<", len))
		cur->type = HEREDOC;
	else if (ft_strlen(only_token) == 1 && !ft_strncmp(str, "<", len))
		cur->type = INPUT;
	else if (ft_strlen(only_token) == 2 && !ft_strncmp(str, ">>", len))
		cur->type = APPEND;
	else if (ft_strlen(only_token) == 1 && !ft_strncmp(str, ">", len))
		cur->type = OVERWRITE;
	else
		cur->type = WORD;
	free(only_token);
}

void	tokenise(t_data *data)
{
	int	i;

	i = 0;
	while (data->line[i])
	{
		while (data->line[i] && ((data->line[i] >= 7 && data->line[i] <= 13)
				|| data->line[i] == ' '))
			i++;
		if (data->line[i])
		{
			add_token(data, i);
			i += get_token_len(data->line + i);
		}
		if (!data->line[i])
			return ;
	}
}
