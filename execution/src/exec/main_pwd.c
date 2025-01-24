/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 07:56:31 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/24 17:07:47 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_find_pwd()
{
	char	buffer[BUFFER_SIZE];

	if (getcwd(buffer, BUFFER_SIZE))
		return (ft_strdup(buffer));
	return (NULL);
}

int	ft_pwd(t_data *data)
{
	printf("%s\n", data->pwd);
	return (0);
}
