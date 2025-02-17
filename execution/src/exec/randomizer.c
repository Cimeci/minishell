/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:38:59 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/13 14:51:00 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*convert_ascii(char *str)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (ft_isalnum(str[i]))
			j++;
		i++;
	}
	line = malloc(sizeof(char) * (j + 1));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (ft_isalnum(str[i]))
			line[j++] = str[i];
		i++;
	}
	line[j] = '\0';
	return (line);
}

int	open_random(void)
{
	int	fd;

	fd = open("/dev/urandom", W_OK);
	if (fd < 0)
	{
		printf("Error fd\n");
		exit(1);
	}
	return (fd);
}

char	*randomizer(void)
{
	int		fd;
	char	*pathname;
	char	*line;

	fd = open_random();
	line = get_next_line(fd);
	while (line)
	{
		pathname = convert_ascii(line);
		free(line);
		if (ft_strlen(pathname) < 250)
		{
			if (access(pathname, F_OK) <= 0)
			{
				close(fd);
				return (pathname);
			}
		}
		free(pathname);
		line = get_next_line(fd);
	}
	close(fd);
	return (NULL);
}
