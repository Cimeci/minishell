/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/09 11:28:11 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_node	*ft_lstnew_generic(size_t data_size)
{
	t_node	*new_node;
	new_node = (t_node *)malloc(sizeof(t_node));
	if (!new_node)
		return (NULL);

	new_node->data = malloc(data_size);
	if (new_node->data == NULL)
	{
		free(new_node);
		return NULL;
	}
	memset(new_node->data, 0, data_size);
	new_node->next = NULL;
	return (new_node->data);
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
		return;
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
			if ((str[0] == '>' && str[i + 1] == '>')
				|| (str[0] == '<' && str[i + 1] == '<'))
				i++;
			break ;
		}
		i++;
	}
	return (i);
}

int	get_token_len(t_token *current, char *str)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	if (str[i] == 34)
	{
		current->quotes[1] = true;
		i++;
		while (str[i + len] && str[i + len] != 34)
			len++;
	}
	else if (str[i] == 39)
	{
		current->quotes[0] = true;
		i++;
		while (str[i + len] && str[i + len] != 39)
			len++;
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

void	get_token(char *str, t_token *current)
{
	int		len;
	char	*only_token;

	len = get_token_len(current, str);
	if (current->quotes[0] || current->quotes[1])
		only_token = ft_substr(str, 1, len);
	else
		only_token = ft_substr(str, 0, len);
	if (!access(str, X_OK) || find_path(only_token))
		current->type = CMD;
	else if (ft_strnstr(str, "|", len))
		current->type = PIPE;
	else if (ft_strnstr(str, "<<", len))
		current->type = HEREDOC;
	else if (ft_strnstr(str, "<", len))
		current->type = INPUT;
	else if (ft_strnstr(str, ">>", len))
		current->type = APPEND;
	else if (ft_strnstr(str, ">", len))
		current->type = OVERWRITE;
	else if (ft_strnstr(str, "$", len))
		current->type = VAR_ENV;
	else
		current->type = ARG;
	free(only_token);
}

void	add_token(t_data *data, t_token *current, int i)
{
	int		len;
	char	*str;

	if (!current)
		return ;
	len = get_token_len(current, data->line + i);
	if (current->quotes[0] || current->quotes[1])
		str = ft_substr(data->line, i + 1, len);
	else
		str = ft_substr(data->line, i, len);
	if (str[0] == '$')
	{
		current->str = ft_strtrim(str, "$");
		free(str);
	}
	else
		current->str = str;
	ft_lstadd_back_generic((void **)&data->token, current, (sizeof(t_token) - sizeof(t_token *)));
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
	t_token	*current;

	i = 0;
	current = NULL;
	while (data->line)
	{
		while (data->line[i])
		{
			while (data->line[i] && data->line[i] == ' ')
				i++;
			if (data->line[i])
			{
				current = (t_token *)ft_lstnew_generic(sizeof(t_token));
				get_token(data->line + i, current);
				add_token(data, current, i);
				if (current->quotes[0] || current->quotes[1])
					i += get_token_len(current, data->line + i) + 2;
				else
					i += get_token_len(current, data->line + i);
			}
			if (!data->line[i])
				return ;
		}
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
	t_lst	*current;

	i = 0;
	data->line = NULL;
	data->token = NULL;
	data->cmd = NULL;
	data->env = NULL;
	current = data->env;
	while (env[i])
	{
		current = malloc(sizeof(t_lst));
		if (!dup)
			return ;
		current->str = ft_strdup(env[i]);
		current->next = NULL;
		ft_lstadd_back_generic((void **)&data->env, current, sizeof(char *));
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
