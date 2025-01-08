/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/08 09:25:41 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *get_line()
{
    char *line;

    line = NULL;
    if (line)
    {
        free(line);
        line = NULL;
    }
    line = readline("Minishell>");
    if (line)
        add_history(line);
    return (line);
}

void    sig_handler(int signum)
{
    if (signum == SIGINT)
    {
        printf("\nctrl C\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    if (signum == SIGQUIT)
    {
        printf("\nctrl \\\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

int main(void)
{
    char    *line;

    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);
    line = get_line();
    printf("%s\n", line);
}