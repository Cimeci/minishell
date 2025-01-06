/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 09:16:49 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/06 09:43:57 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char	*input;

    input = "1";
    while (input)
    {
        input = readline("User> ");
		if (input)
		{
			add_history(input);
            printf("Vous avez saisi : %s\n", input);
            free(input);
		}
	}
	return (0);
}
