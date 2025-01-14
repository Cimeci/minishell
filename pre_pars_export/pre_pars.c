/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_pars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 09:18:26 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/14 10:41:54 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	if (table)
	{
		while (table[i])
			free(table[i++]);
		free(table);
		table = NULL;
	}
}

void	printf_tab(char **table, char *msg)
{
	int	w;

	w = 0;
	while (table[w])
		printf("%s | %s\n", msg, table[w++]);
}

char	*get_define(char *argv)
{
	char	**table;
	char	**var_name;
	char	**var;
	char	*tmp;
	char	*result;
	char	*final_result;
	char	*after_egual;
	int		i;
	int		j;

	if (argv[0] == '$' && argv)
		return (NULL);
	table = ft_split(argv, '=');
	after_egual = ft_strnstr(argv, "=", ft_strlen(argv));
	var_name = ft_split(table[0], '$');
	i = 0;
	j = 0;
	while (var_name[i])
		i++;
	var = malloc(sizeof(char *) * (i + 1));
	if (!var)
		return (NULL);
	i = 0;
	j = 0;
	while (var_name[i])
	{
		tmp = getenv(var_name[i]);
		if (tmp)
			var[j] = ft_strdup(tmp);
		else
			var[j] = ft_strdup(var_name[i]);
		j++;
		i++;
	}
	var[j] = NULL;
	i = 0;
	result = var[i];
	i++;
	while (var[i])
	{
		tmp = ft_strdup(result);
		result = ft_strjoin(tmp, var[i]);
		i++;
	}
	final_result = ft_strjoin(result, after_egual);
	ft_free_tab(table);
	ft_free_tab(var_name);
	free(tmp);
	free(result);
	ft_free_tab(var);
	return (final_result);
}

char	**get_var_define(char **argv)
{
	int		k;
	char	**result;
	char	*tmp;
	int		len;
	int		j;

	len = 0;
	k = 1;
	while (argv[k])
	{
		tmp = get_define(argv[k]);
		if (tmp)
			len++;
		k++;
		free(tmp);
	}
	result = malloc(sizeof(char *) * (len + 1));
	if (!result)
		return (NULL);
	k = 1;
	j = 0;
	while (argv[k])
	{
		tmp = get_define(argv[k]);
		if (tmp)
		{
			result[j] = ft_strdup(tmp);
			j++;
		}
		else
			printf("Error\n");
		k++;
	}
	result[j] = NULL;
	free(tmp);
	return (result);
}

int	main(int argc, char **argv)
{
	char	*input;
	char	**result;

	if (argc != 1)
	{
		printf("Error input\n");
		exit(1);
	}
	input = "1";
	while (input)
	{
		input = readline("Minishell> ");
		if (input && input[0] != '\0')
		{
			add_history(input);
			argv = ft_split(input, ' ');
			if (argv)
			{
				result = get_var_define(argv);
				if (result)
				{
					printf_tab(result, "result");
					ft_free_tab(result);
				}
				ft_free_tab(argv);
				argv = NULL;
			}
		}
		if (input)
			free(input);
	}
}
