/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 15:09:27 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define BUFFER_SIZE 100000

char	*ft_clear_quote(char *input)
{
	int		i;
	int		j;
	char	*result;
	int		re;

	j = 0;
	i = 0;
	while (input[i] != '"' && input[i])
		i++;
	if (input[i] == '"')
	{
		i++;
		j = i;
		while (input[i + 1] != '"' && input[i])
			i++;
		result = ft_substr(input, j, i);
		re = ft_strncmp(result, "pwd", i);
		if (re)
		{
			printf("%s: too many argument5s\n", input);
			return (NULL);
		}
	}
	return (ft_strtrim(input, "\""));
}

char	*ft_pwd(char **argv)
{
	char	buffer[BUFFER_SIZE];

	if (!ft_strncmp(argv[0], "pwd", ft_strlen(argv[0])))
	{
		if (getcwd(buffer, BUFFER_SIZE) == NULL)
		{
			printf("Error directory\n");
			if (errno == ERANGE)
				printf("BUFFER_SIZE too small\n");
			return (NULL);
		}
		printf("%s\n", buffer);
		return (ft_strdup(buffer));
	}
	return (NULL);
}
