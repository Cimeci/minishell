/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:38:59 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/20 11:47:48 by ncharbog         ###   ########.fr       */
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
	char	*filename;
	char	buffer[1000];

	filename = NULL;
	fd = open_random();
	while (1)
	{
		read(fd, buffer, 1000);
		filename = convert_ascii(buffer);
		if (ft_strlen(filename) < 250)
		{
			if (access(filename, F_OK) <= 0)
			{
				close(fd);
				if (filename)
					return (filename);
				return (NULL);
			}
		}
		free(filename);
	}
	close(fd);
	return (NULL);
}
