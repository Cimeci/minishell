/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:34:49 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/10 10:48:46 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quotes(char *str, t_token *cur)
{
	char	*tmp;
	char	quote;
	int		i;
	int		start;
	int		len;

	i = 0;
	len = 0;
	start = 0;
	tmp = NULL;
	while (str[i])
	{
		start = i;
		while (str[i + len] && !IS_QUOTE(str[i + len]))
			len++;
		tmp = ft_substr(str, start, len);
		cur->str = ft_strjoin_free(cur->str, tmp);
		if (tmp)
			free(tmp);
		tmp = NULL;
		i += len;
		len = 0;
		if (str[i] && IS_QUOTE(str[i]))
		{
			quote = str[i];
			i++;
			while (str[i + len] && str[i + len] != quote)
				len++;
			tmp = ft_substr(str, i, len);
			cur->str = ft_strjoin_free(cur->str, tmp);
			if (tmp)
				free(tmp);
			tmp = NULL;
			i += len + 1;
		}
		len = 0;
	}
	if (tmp)
		free(tmp);
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
			if ((str[0] == '>' && str[i] == '>')
				|| (str[0] == '<' && str[i] == '<'))
				i++;
			break ;
		}
		i++;
	}
	return (i);
}

void	add_token(t_data *data, t_token *cur, int i)
{
	t_token	*last;
	int		len;
	char	*str;

	if (!cur)
		return ;
	cur->str = malloc(1);
	cur->str[0] = '\0';
	last = ft_lstlast_generic(data->token, (sizeof(t_token) - sizeof (t_token *)));
	len = get_token_len(data->line + i);
	str = ft_substr(data->line, i, len);
	if (ft_strchr(str, '$') && (!last || last->type != HEREDOC))
		str = env_variables(data, str);
	remove_quotes(str, cur);
	free(str);
	ft_lstadd_back_generic((void **)&data->token, cur, (sizeof(t_token) - sizeof(t_token *)));
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
	else if (ft_strlen(only_token) == 2
		&& (!ft_strncmp(str, "''", len) || !ft_strncmp(str, "\"\"", len)))
		cur->type = EMPTY_QUOTE;
	else
		cur->type = WORD;
	free(only_token);
}

void	tokenise(t_data *data)
{
	int		i;
	t_token	*cur;

	i = 0;
	cur = NULL;
	while (data->line[i])
	{
		while (data->line[i] && data->line[i] == ' ')
			i++;
		if (data->line[i])
		{
			cur = (t_token *)ft_lstnew_generic(sizeof(t_token));
			if (!cur)
				return ;
			get_token(data->line + i, cur);
			add_token(data, cur, i);
			i += get_token_len(data->line + i);
		}
		if (!data->line[i])
			return ;
	}
}
