/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/23 17:56:07 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_absolut_path(t_data *data, t_cmd *cur)
{
	char	*path;
	char	*target_path;

		target_path = ft_strsstr(data->pwd, cur->args[1]);
		if (!target_path)
		{
			path = ft_strjoin(data->pwd, "/");
			target_path = ft_strjoin(path, cur->args[1]);
		}
		if (chdir(target_path))
		{
			ft_free_path(path, target_path);
			perror("Error");
			return ;
		}
		// printf("%s\n", target_path);
		// printf("%s\n", ft_find_pwd());
		if (target_path)
			free(target_path);
}

int	ft_cd(t_data *data, t_cmd *cur)
{
	char	*root;

	root = NULL;
	if (cur->args[1][0] == '\0' || ft_strlen_tab(cur->args) != 2)
	{
		ft_putendl_fd("Error : No path", 2);
		return (-1);
	}
	if (cur->args[1] && cur->args[1][0] != '\0')
	{
		if (!ft_strncmp(cur->args[1], "/", ft_strlen(cur->args[1])))
		{
			data->pwd = ft_strdup("/");
			if (chdir("/"))
			{
				perror("Error");
				return (-1);
			}
			return (1);
		}
		ft_absolut_path(data, cur);
		return (1);
	}
	return (0);
}
