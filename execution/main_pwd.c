/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/15 16:39:33 by inowak--         ###   ########.fr       */
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

int	ft_pwd(char **argv)
{
	char	buffer[BUFFER_SIZE];
	int		fd;

	fd = open_output_file(argv, ft_strlen_tab(argv));
	if (!ft_strncmp(argv[0], "pwd", ft_strlen(argv[0])))
	{
		if (getcwd(buffer, BUFFER_SIZE) == NULL)
		{
			printf("Error directory\n");
			if (errno == ERANGE)
				printf("BUFFER_SIZE too small\n");
			return (-1);
		}
		ft_putendl_fd(buffer, fd);
		return (1);
	}
	return (0);
}
