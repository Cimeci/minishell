/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:38:59 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/21 16:37:54 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*convert_ascii(char *str)
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
	if (!line)
		return (NULL);
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

static int	open_random(void)
{
	int	fd;

	fd = open("/dev/urandom", W_OK);
	if (fd < 0)
	{
		printf("Error fd\n");
		return (-1);
	}
	return (fd);
}

char	*randomizer(void)
{
	int		fd;
	char	*filename;
	char	buffer[1000];

	filename = NULL;
	fd = open_random();
	if (fd < 0)
		return (NULL);
	while (1)
	{
		read(fd, buffer, 1000);
		filename = convert_ascii(buffer);
		if (filename && (ft_strlen(filename) < 50))
		{
			if (access(filename, F_OK))
			{
				close(fd);
				return (filename);
			}
		}
		free(filename);
	}
	close(fd);
	return (NULL);
}
