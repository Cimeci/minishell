/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/07 14:48:08 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	*ft_lstnew2(void *content)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->str = content;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back2(t_token **lst, t_token *new)
{
	t_token	*temp;

	if (!lst || !new)
		return ;
	temp = *lst;
	if (!temp)
	{
		*lst = new;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
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
	}
	else if (str[i] == 39)
	{
		i++;
		while (str[i] && str[i] != 39)
			i++;
	}
	else
	{
		while (str[i] && str[i] != ' ')
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
	ft_lstadd_back2(&(data->token), current);
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
	current = data->token;
	while (data->line)
	{
		while (data->line[i])
		{
			while (data->line[i] && data->line[i] == ' ')
				i++;
			if (data->line[i])
			{
				current = ft_lstnew2(NULL);
				get_token(data->line + i, current);
				add_token(data, current, i);
				i += get_token_len(data->line + i);
			}
			if (!data->line[i])
				return ;
			current = current->next;
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
	t_token *cur;
	cur = data->token;
	while (cur)
	{
		printf("%s | %d\n", cur->str, cur->type);
		cur = cur->next;
	}
}

void	ft_init_data(t_data *data)
{
	data->line = NULL;
	data->token = NULL;
	data->cmd = NULL;
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
		ft_free_lst(&(data->token));
		data->token = NULL;
	}
}

int main(int argc, char **env)
{
	t_data	data;

	(void)env;
	if (argc != 1)
	{
		printf("toooooo many arguments bro\n");
		return (1);
	}
	ft_init_data(&data);
	prompt(&data);
	ft_free_lst(&(data.token));
}
