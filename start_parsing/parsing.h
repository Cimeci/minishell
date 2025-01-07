/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:31 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/07 16:28:56 by ncharbog         ###   ########.fr       */
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

typedef enum e_type
{
	INPUT, //<
	HEREDOC, //<<
	OVERWRITE, //>
	APPEND, //>>
	PIPE,
	CMD,
	ARG,
}	t_type;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_token
{
	char	*str;
	int		type;
	struct s_token	*next;
}			t_token;

typedef struct s_lst
{
	char	*str;
	struct s_lst	*next;
}	t_lst;

typedef struct s_data
{
	char	*line;
	t_token	*token;
	t_cmd	*cmd;
	t_list	*env;
}			t_data;


void	ft_free_tab(char **table);
void	ft_free_lst(t_token **head);

#endif