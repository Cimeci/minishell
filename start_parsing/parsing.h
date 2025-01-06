/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:31 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/06 17:53:46 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../libft/libft.h"

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <term.h>
# include <errno.h>

// # define CMD 1
// # define ARG 2
// # define OPERATOR 3

typedef enum e_type
{
	CMD,
	ARG,
	OPERATOR
}	t_type;

typedef struct s_token
{
	char	*str;
	int		type;
	struct s_token	*next;
}			t_token;

typedef struct s_data
{
	char *line;
	t_token *token;
}			t_data;


void	ft_free_tab(char **table);
void	ft_free_lst(t_token **head);

#endif