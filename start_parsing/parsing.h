/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:31 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/16 12:05:06 by ncharbog         ###   ########.fr       */
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
# include <stdbool.h>

enum e_type
{
	INPUT, //<
	HEREDOC, //<<
	OVERWRITE, //>
	APPEND, //>>
	PIPE,
	WORD,
}	;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	char			*infile;
	char			*outfile;
	int				flag_redir;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_token
{
	char	*str;
	int		type;
	bool	quotes;
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
	t_lst	*env;
}			t_data;

# define DOUBLE_QUOTE 34
# define SINGLE_QUOTE 39
# define IS_QUOTE(c) (c == DOUBLE_QUOTE || c == SINGLE_QUOTE)
# define IS_SEPARATOR(c) ((c) == '=' || (c) == '$' || (c) == '>' || (c) == '<' || (c) == '|' || (c) == ' ' || (c) == '\'' || (c) == '"')
# define IS_SEPARATOR_TOKEN(c) ((c) == '>' || (c) == '<' || (c) == '|' || (c) == ' ')

void	ft_free_tab(char **table);
void	free_token(t_token **head);
void	free_env(t_lst **head);
void	free_all(t_data *data);
char	*ft_strjoin_free(char *s1, char *s2);
void	free_cmd(t_cmd **head);

#endif