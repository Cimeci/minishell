/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/16 12:08:59 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	*ft_lstnew_generic(size_t data_size)
{
	void	*new_node;

	new_node = malloc(data_size);
	if (!new_node)
		return (NULL);
	memset(new_node, 0, data_size);
	return (new_node);
}

void ft_lstadd_back_generic(void **lst, void *new_node, size_t next_offset)
{
	void	*temp;

	if (!lst || !new_node)
		return;
	temp = *lst;
	if (*lst == NULL)
	{
		*lst = new_node;
		*(void **)((char *)new_node + next_offset) = NULL;
		return ;
	}
	while (*(void **)((char *)temp + next_offset) != NULL)
		temp = *(void **)((char *)temp + next_offset);
	if (temp)
		*(void **)((char *)temp + next_offset) = new_node;
}

int	get_token_len(t_token *cur, char *str)
{
	int		i;
	char	quote;

	i = 0;
	(void)cur;
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

char	*find_path(char *str)
{
	char	*path;
	char	**path_split;
	char	*pathname;
	char	*tmp;
	int		i;

	i = 0;
	path = getenv("PATH");
	path_split = ft_split(path, ':');
	while (path_split[i])
	{
		tmp = ft_strjoin(path_split[i], "/");
		pathname = ft_strjoin(tmp, str);
		free(tmp);
		if (!access(pathname, X_OK))
		{
			ft_free_tab(path_split);
			return (pathname);
		}
		free(pathname);
		i++;
	}
	ft_free_tab(path_split);
	return (NULL);
}

void	get_token(char *str, t_token *cur)
{
	int		len;
	char	*only_token;

	len = get_token_len(cur, str);
	only_token = ft_substr(str, 0, len);
	printf("only_token = %s\n", only_token);
	if (ft_strnstr(str, "|", len))
		cur->type = PIPE;
	else if (ft_strnstr(str, "<<", len))
		cur->type = HEREDOC;
	else if (ft_strnstr(str, "<", len))
		cur->type = INPUT;
	else if (ft_strnstr(str, ">>", len))
		cur->type = APPEND;
	else if (ft_strnstr(str, ">", len))
		cur->type = OVERWRITE;
	else
		cur->type = WORD;
	free(only_token);
}

char	*my_getenv(t_data *data, char *name)
{
	t_lst	*cur;
	char	*tmp;
	int		i;

	cur = data->env;
	i = 0;
	while (cur)
	{
		i = 0;
		while (cur->str[i] && cur->str[i] != '=')
			i++;
		tmp = ft_substr(cur->str, 0, i);
		if (ft_strncmp(tmp, name, i) == 0 && name[i] == '\0')
		{
			free(tmp);
			return (cur->str + i + 1);
		}
		cur = cur->next;
		free(tmp);
	}
	return (NULL);
}

char	*remove_char(char *str, char c)
{
	char	**table;
	char	*dest;
	char	*tmp;
	int		i;

	table = ft_split(str, c);
	dest = ft_strjoin(table[0], table[1]);
	i = 2;
	while (table[i])
	{
		tmp = dest;
		dest = ft_strjoin(tmp, table[i++]);
		free(tmp);
	}
	return (dest);
}

int	*expansion_quotes(char *line, int nb_var)
{
	int		i;
	int		j;
	int		*quote_tab;
	char	quote;

	i = 0;
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

void	env_variables(t_data *data)
{
	int		i;
	int		j;
	int		*quote_tab;
	char	**table;
	char	*var;
	char	*tmp;

	i = 0;
	j = 0;
	table = ft_split(data->line, '$');
	while (table[i])
		i++;
	quote_tab = expansion_quotes(data->line, i);
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
	data->line = table[0];
	i = 1;
	while (table[i])
		data->line = ft_strjoin_free(data->line, table[i++]);
}

void	add_token(t_data *data, t_token *cur, int i)
{
	int		len;
	char	*str;

	if (!cur)
		return ;
	len = get_token_len(cur, data->line + i);
	str = ft_substr(data->line, i, len);
	if (ft_strchr(str, '"'))
		cur->str = remove_char(str, '"');
	if (ft_strchr(str, '\''))
		cur->str = remove_char(str, '\'');
	else
		cur->str = str;
	ft_lstadd_back_generic((void **)&data->token, cur, (sizeof(t_token) - sizeof(t_token *)));
}

int	check_quotes(t_data *data)
{
	int	i;

	i = 0;
	while (data->line[i])
	{
		if (data->line[i] == '\'')
		{
			i++;
			while (data->line[i] && data->line[i] != '\'')
				i++;
			if (data->line[i] != '\'')
				return (-1);
		}
		if (data->line[i] == '"')
		{
			i++;
			while (data->line[i] && data->line[i] != '"')
				i++;
			if (data->line[i] != '"')
				return (-1);
		}
		i++;
	}
	return (0);
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
			i += get_token_len(cur, data->line + i);
		}
		if (!data->line[i])
			return ;
	}
}

t_token	*redir_cmd(t_cmd *cur_cmd, t_token *cur_tok)
{
	if (cur_tok->type == INPUT)
	{
		cur_cmd->infile = ft_strdup(cur_tok->next->str);
		cur_tok = cur_tok->next->next;
	}
	else if (cur_tok->type == OVERWRITE || cur_tok->type == APPEND)
	{
		cur_cmd->outfile = ft_strdup(cur_tok->next->str);
		if (cur_tok->type == APPEND)
			cur_cmd->flag_redir = 1;
		cur_tok = cur_tok->next->next;
	}
	return (cur_tok);
}

t_token	*build_cmd(t_cmd *cur_cmd, t_token *cur_tok)
{
	t_token	*tmp;
	int		i;
	int		len;

	i = 0;
	len = 0;
	tmp = cur_tok;
	cur_cmd->cmd = find_path(cur_tok->str);
	if (!cur_cmd->cmd)
		cur_cmd->cmd = ft_strdup(cur_tok->str);
	while (tmp && tmp->type != PIPE)
	{
		if (cur_tok->type == INPUT || cur_tok->type == APPEND
			|| cur_tok->type == OVERWRITE)
			tmp = tmp->next->next;
		len++;
		tmp = tmp->next;
	}
	cur_cmd->args = malloc((len + 1) * sizeof(char *));
	if (!cur_cmd->args)
		return (NULL);
	while (cur_tok && cur_tok->type != PIPE)
	{
		if (cur_tok->type == INPUT || cur_tok->type == APPEND
			|| cur_tok->type == OVERWRITE)
			cur_tok = redir_cmd(cur_cmd, cur_tok);
		else
		{
			cur_cmd->args[i] = ft_strdup(cur_tok->str);
			i++;
			cur_tok = cur_tok->next;
		}
	}
	cur_cmd->args[i] = NULL;
	return (cur_tok);
}

void	get_cmds(t_data *data)
{
	t_cmd	*cur_cmd;
	t_token	*cur_tok;

	cur_cmd = data->cmd;
	cur_tok = data->token;
	while (cur_tok)
	{
		cur_cmd = (t_cmd *)ft_lstnew_generic(sizeof(t_cmd));
		if (!cur_cmd)
			return ;
		while (cur_tok && cur_tok->type != PIPE)
		{
			if (cur_tok->type == INPUT || cur_tok->type == APPEND
				|| cur_tok->type == OVERWRITE)
				cur_tok = redir_cmd(cur_cmd, cur_tok);
			else if (cur_tok->type == WORD)
				cur_tok = build_cmd(cur_cmd, cur_tok);
		}
		ft_lstadd_back_generic((void **)&(data->cmd), cur_cmd, (sizeof(t_cmd) - sizeof(t_cmd *)));
		if (cur_tok)
			cur_tok = cur_tok->next;
	}
}

void	parsing(t_data *data)
{
	if (check_quotes(data) == -1)
	{
		printf("erreur quote");
		exit(EXIT_FAILURE);
	}
	env_variables(data);
	tokenise(data);
	//get_cmds(data);
}

void	init_data(t_data *data, char **env)
{
	int		i;
	t_lst	*cur;

	i = 0;
	data->line = NULL;
	data->token = NULL;
	data->cmd = NULL;
	data->env = NULL;
	cur = data->env;
	while (env[i])
	{
		cur = (t_lst *)ft_lstnew_generic(sizeof(t_lst));
		if (!cur)
			return ;
		cur->str = ft_strdup(env[i]);
		cur->next = NULL;
		ft_lstadd_back_generic((void **)&data->env, cur, sizeof(char *));
		i++;
	}
}

void	prompt(t_data *data)
{
	char	*input;

	input = "1";
	while (input)
	{
		input = readline("minishell> ");
		if (input)
		{
			add_history(input);
			data->line = input;
			parsing(data);
			free(input);
		}
		t_cmd	*cur;
		int		i = 0;
		cur = data->cmd;
		while (cur)
		{
			printf("cmd : %s\n", cur->cmd);
			i = 0;
			while (cur->args[i])
			{
				printf("args : %s\n", cur->args[i]);
				i++;
			}
			printf("outfile : %s\n", cur->outfile);
			printf("infile : %s\n", cur->infile);
			cur = cur->next;
			printf ("\n");
		}
		free_token(&data->token);
		free_cmd(&data->cmd);
		data->token = NULL;
	}
}

int main(int argc, char **argv, char **env)
{
	t_data	data;

	(void)argv;
	if (argc != 1)
	{
		printf("too many arguments\n");
		return (1);
	}
	init_data(&data, env);
	prompt(&data);
	free_all(&data);
}
