/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:38:59 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/28 09:38:09 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*convert_ascii(char *str)
{
	int	i;
	int	j;
	char	*line;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (ft_isascii(str[i]))
			j++;
		i++;
	}
	line = malloc(sizeof(char) * (j + 1));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (ft_isascii(str[i]))
		{
			// printf("|%c|", str[i]);
			line[j] = str[i];
			j++;
		}
		i++;
	}
	line[j] = '\0';
	// printf("\n\n");
	return (line);
}

char	*randomizer(void)
{
	int		fd;
	char	*pathname;
	char	*line;

	fd = open("/dev/urandom", W_OK);
	if (fd < 0)
	{
		printf("Error fd\n");
		exit(0);
	}
	line = get_next_line(fd);
	while (line)
	{
		pathname = convert_ascii(line);
		// printf("%s\n", pathname);
		if (ft_strlen(pathname) < 250)
		{
			if (access(pathname, F_OK))
				return (pathname);
		}
		line = get_next_line(fd);
	}
	return (NULL);
}