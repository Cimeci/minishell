/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/06 17:54:03 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

			// if (data->str[i] == 34 && double_quote > 0)
			// 	double_quote++;
			// if (data->str[i] == 39 && quote > 0)
			// 	quote++;
			// if (data->str[i] == 34)
			// 	double_quote++;
			// if (data->str[i] == 39)
			// 	quote++;

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

int		get_token_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ' ')
		i++;
	return (i);
}

char *find_path(char *str)
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
		i++;
	}
	free(pathname);
	ft_free_tab(path_split);
	return (NULL);
}

void	get_token(char *str, t_token *current)
{
	int	len;

	len = get_token_len(str);
	if (!access(str, X_OK) || find_path(str))
		current->type = CMD;
	else if (ft_strnstr(str, "|", len) || ft_strnstr(str, "<", len)
		|| ft_strnstr(str, ">", len) || ft_strnstr(str, ">>", len)
		|| ft_strnstr(str, "<<", len))
		current->type = OPERATOR;
	else
		current->type = ARG;
}

void	add_token(t_data *data, int i)
{
	int		len;
	char	*str;

	len = get_token_len(data->line + i);
	str = ft_substr(data->line, i, len);
	data->token->str = str;
	printf("%s|%d|\n", str, data->token->type);
}

void	parsing(t_data *data)
{
	int		i;

	i = 0;
	while (data->line)
	{
		while (data->line[i])
		{
			while (data->line[i] && data->line[i] == ' ')
				i++;
			if (data->line[i])
			{
				data->token = ft_lstnew2(NULL);
				get_token(data->line + i, data->token);
				add_token(data, i);
				while (data->line[i] && data->line[i] != ' ')
					i++;
			}
			if (!data->line[i])
				return ;
			data->token = data->token->next;
		}
	}
}

t_data	ft_init_data(void)
{
	t_data *data;

	data = malloc(sizeof(t_data));
	if (!data)
		exit (1);
	data->line = NULL;
	data->token = NULL;
	return (*data);
}

void	get_prompt(t_data *data)
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
	data = ft_init_data();
	get_prompt(&data);
	ft_free_lst(&(data.token));
}
