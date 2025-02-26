/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:13 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/26 09:47:40 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	int		last_exit;

	(void)argv;
	if (argc != 1)
	{
		printf("minishell: Too many arguments\n");
		return (1);
	}
	init_data(&data, env);
	prompt(&data);
	last_exit = data.gexit_code;
	free_all(&data, 0);
	exit(last_exit);
}
