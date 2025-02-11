/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 10:49:30 by inowak--          #+#    #+#             */
/*   Updated: 2025/02/11 11:24:17 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*my_getenv(t_data *data, char *name)
{
	t_lst	*cur;
	char	*tmp;
	int		i;

	cur = data->env;
	i = 0;
	while (cur)
	{
		i = 0;
		while (cur->str[i] && cur->str[i] != '=')
			i++;
		tmp = ft_substr(cur->str, 0, i);
		if (ft_strncmp(tmp, name, i) == 0 && name[i] == '\0')
		{
			free(tmp);
			return (cur->str + i + 1);
		}
		cur = cur->next;
		free(tmp);
	}
	return (NULL);
}

char	*find_path(t_data *data, char *str)
{
	char	*path;
	char	**path_split;
	char	*pathname;
	char	*tmp;
	int		i;

	i = 0;
	path = my_getenv(data, "PATH");
	if (!path)
		return (NULL);
	path_split = ft_split(path, ':');
	while (path_split[i])
	{
		tmp = ft_strjoin(path_split[i], "/");
		pathname = ft_strjoin(tmp, str);
		free(tmp);
		if (!access(pathname, X_OK))
		{
			ft_free_tab(path_split);
			return (pathname);
		}
		free(pathname);
		i++;
	}
	ft_free_tab(path_split);
	return (NULL);
}
