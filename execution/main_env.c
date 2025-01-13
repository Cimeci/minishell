/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 15:10:08 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **argv, t_env *env)
{
	t_env	*cur;

	cur = env;
	if (!ft_strncmp(argv[0], "env", ft_strlen(argv[0])))
	{
		while (cur)
		{
			printf("%s\n", cur->path);
			cur = cur->next;
		}
		return (1);
	}
	return (0);
}
