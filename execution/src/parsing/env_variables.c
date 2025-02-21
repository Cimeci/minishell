/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:32:27 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/21 12:51:09 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*fill_quote_tab(char *line, int *quote_tab, char quote, bool heredoc)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == SINGLE_QUOTE || line[i] == DOUBLE_QUOTE)
		{
			quote = line[i];
			i++;
			while (line[i] && line[i] != quote)
			{
				if (line[i] == '$' && (quote == '\'' && heredoc == false))
					quote_tab[j++] = 0;
				if (line[i] == '$' && (quote == '"' || heredoc == true))
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

int	*expansion_quotes(char *line, int nb_var, bool heredoc)
{
	int		i;
	int		j;
	int		*quote_tab;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	quote_tab = malloc(nb_var * sizeof(int));
	if (!quote_tab)
		return (0);
	quote_tab = fill_quote_tab(line, quote_tab, quote, heredoc);
	return (quote_tab);
}

char	*add_env_token(t_data *data, char *line, t_env_var *info, int flag_expand)
{
	t_token	*new;

	free(info->quote_tab);
	if (info->heredoc == true)
	{
		free(info);
		return (line);
	}
	free(info);
	new = (t_token *)ft_lstnew_generic(sizeof(t_token));
	if (!new)
		errors(data, NULL, MALLOC);
	new->type = WORD;
	if (line[0] == '\0')
		new->empty_var_tok = true;
	new->expand = flag_expand;
	new->str = remove_quotes(data, line);
	free(line);
	ft_lstadd_back_generic((void **)&data->token, new,
		(sizeof(t_token) - sizeof(t_token *)));
	return (NULL);
}

void	init_info(t_env_var **info, char *line, bool heredoc)
{
	*info = malloc(sizeof(t_env_var));
	if (!*info)
		return ;
	(*info)->i = 0;
	(*info)->stop = false;
	(*info)->dollars = count_char(line, '$');
	(*info)->quote_tab = expansion_quotes(line, (*info)->dollars, heredoc);
	(*info)->heredoc = heredoc;
	(*info)->dollars = 0;
}

char	*env_variables(t_data *data, char *line, bool heredoc)
{
	t_env_var	*info;

	info = NULL;
	init_info(&info, line, heredoc);
	while (info->i < ft_strlen(line) && line[info->i])
	{
		if (line[info->i] == '$')
		{
			line = only_dollars(data, line, info);
			while (line[info->i] && !is_separator_env(line[info->i], 1))
				info->i++;
			if (line[info->i] == '$')
			{
				line = is_var(data, line, info);
				if ((!line || line[0] == '\0') && info->stop == true)
					return (NULL);
				if (!line[info->i])
					break ;
			}
		}
		else
			info->i++;
	}
	return (add_env_token(data, line, info, true));
}
