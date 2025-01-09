/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/09 17:23:31 by ncharbog         ###   ########.fr       */
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

int	next_separator(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '<' || str[i] == '|' || str[i] == '>')
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

int	get_token_len(t_token *cur, char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if (IS_QUOTE(str[i]))
	{
		if (str[i] == DOUBLE_QUOTE)
			cur->quotes[1] = true;
		else
			cur->quotes[0] = true;
		i++;
		while (str[i + len] && str[i + len] != str[0])
			len++;
		if (str[i + len + 1] && str[i + len + 1] != ' ')
		{
			while (str[i + len] && str[i + len] != ' ')
				len++;
		}
	}
	else
		len = next_separator(str);
	return (len);
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
			free(pathname);
			ft_free_tab(path_split);
			return ("good");
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
	only_token = ft_substr(str, cur->quotes[1] || cur->quotes[0], len);
	if (!access(str, X_OK) || find_path(only_token))
		cur->type = CMD;
	else if (ft_strnstr(str, "|", len) && !cur->quotes[0] && !cur->quotes[1])
		cur->type = PIPE;
	else if (ft_strnstr(str, "<<", len) && !cur->quotes[0] && !cur->quotes[1])
		cur->type = HEREDOC;
	else if (ft_strnstr(str, "<", len) && !cur->quotes[0] && !cur->quotes[1])
		cur->type = INPUT;
	else if (ft_strnstr(str, ">>", len) && !cur->quotes[0] && !cur->quotes[1])
		cur->type = APPEND;
	else if (ft_strnstr(str, ">", len) && !cur->quotes[0] && !cur->quotes[1])
		cur->type = OVERWRITE;
	else if (ft_strnstr(str, "$", len) && !cur->quotes[0])
		cur->type = VAR_ENV;
	else
		cur->type = ARG;
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

void	add_token(t_data *data, t_token *cur, int i)
{
	int		len;
	char	*var;
	char	*str;

	if (!cur)
		return ;
	len = get_token_len(cur, data->line + i);
	str = ft_substr(data->line, i + (cur->quotes[0] || cur->quotes[1]), len);
	if (ft_strchr(str, '"'))
		cur->str = remove_char(str, '"');
	if (ft_strchr(str, '\''))
		cur->str = remove_char(str, '\'');
	else if (str[0] == '$' && !cur->quotes[0])
	{
		var = ft_strtrim(str, "$");
		cur->str = ft_strdup(my_getenv(data, var));
		if (!cur->str)
			cur->str = str;
		else
			free(str);
		free(var);
	}
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
			if (cur->quotes[0] || cur->quotes[1])
				i += get_token_len(cur, data->line + i) + 2;
			else
				i += get_token_len(cur, data->line + i);
		}
		if (!data->line[i])
			return ;
	}
}

void	parsing(t_data *data)
{
	if (check_quotes(data) == -1)
	{
		printf("erreur quote");
		exit(EXIT_FAILURE);
	}
	tokenise(data);
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
		t_token *cur;
		cur = data->token;
		while (cur)
		{
			printf("%s | %d\n", cur->str, cur->type);
			cur = cur->next;
		}
		free_token(&data->token);
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
