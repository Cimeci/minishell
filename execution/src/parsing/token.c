/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:34:49 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/20 17:26:11 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_token_len(char *str)
{
	int		i;
	char	quote;

	i = 0;
	while (str[i])
	{
		if (str[i] == SINGLE_QUOTE || str[i] == DOUBLE_QUOTE)
		{
			quote = str[i];
			i++;
			while (str[i] != quote)
				i++;
		}
		if (is_separator_token(str[i]))
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

void	new_token_node(t_data *data, char *str, int len)
{
	t_token	*new;
	t_token	*last;

	last = ft_lstlast_generic(data->token, (sizeof(t_token)
				- sizeof(t_token *)));
	new = (t_token *)ft_lstnew_generic(sizeof(t_token));
	if (!new)
		errors(data, NULL, MALLOC);
	new->expand = true;
	if ((ft_strnstr(str, "\"", len) || ft_strnstr(str, "'", len)) && last
		&& last->type == HEREDOC)
		new->expand = false;
	get_token(str, new);
	new->str = remove_quotes(data, str);
	new->empty_var_tok = false;
	if (ft_strlen(new->str) == 1 && !ft_strncmp(new->str, ".",
			ft_strlen(new->str)))
		new->type = DOT;
	if (ft_strlen(new->str) == 2 && !ft_strncmp(new->str, "..",
			ft_strlen(new->str)))
		new->type = DOT;
	ft_lstadd_back_generic((void **)&data->token, new, (sizeof(t_token)
			- sizeof(t_token *)));
}

void	add_token(t_data *data, int i)
{
	t_token	*last;
	int		len;
	char	*str;

	last = ft_lstlast_generic(data->token, (sizeof(t_token)
				- sizeof(t_token *)));
	len = get_token_len(data->line + i);
	str = ft_substr(data->line, i, len);
	if (ft_strchr(str, '$') && (!last || last->type != HEREDOC))
		env_variables(data, str, false);
	else
		new_token_node(data, str, len);
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
