/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/08 14:20:04 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

int	get_token_len(char *str)
{
	int	i;

	i = 0;
	if (str[i] == 34)
	{
		i++;
		while (str[i] && str[i] != 34)
			i++;
		i++;
	}
	else if (str[i] == 39)
	{
		i++;
		while (str[i] && str[i] != 39)
			i++;
		i++;
	}
	else
		i = next_separator(str);
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

	len = get_token_len(str);
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
	len = get_token_len(data->line + i);
	str = ft_substr(data->line, i, len);
	if (str[0] == '\'')
	{
		current->str = ft_strtrim(str, "'");
		free(str);
	}
	else if (str[0] == '"')
	{
		current->str = ft_strtrim(str, "\"");
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
	t_token	*tmp;

	i = 0;
	tmp = NULL;
	while (data->line)
	{
		while (data->line[i])
		{
			while (data->line[i] && data->line[i] == ' ')
				i++;
			if (data->line[i])
			{
				tmp = malloc(sizeof(t_token));
				if (!tmp)
					return ;
				tmp->next = NULL;
				get_token(data->line + i, tmp);
				add_token(data, tmp, i);
				i += get_token_len(data->line + i);
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
	t_lst	*dup;

	i = 0;
	data->line = NULL;
	data->token = NULL;
	data->cmd = NULL;
	data->env = NULL;
	dup = data->env;
	while (env[i])
	{
		dup = malloc(sizeof(t_lst));
		if (!dup)
			return ;
		dup->str = ft_strdup(env[i]);
		dup->next = NULL;
		ft_lstadd_back_generic((void **)&data->env, dup, sizeof(char *));
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
