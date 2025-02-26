/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:31 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/26 18:50:29 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"
# include <stdbool.h>

enum				e_type
{
	INPUT,
	HEREDOC,
	OVERWRITE,
	APPEND,
	PIPE,
	WORD,
	DOT,
};

enum				e_error
{
	ERROR_SYNTAX,
	CMD_NOT_FOUND,
	FILES,
	PERM,
	DIRECTORY,
	ARGS,
	IDENTIFIER,
	MALLOC,
	ERRNO,
	NUM_ARG,
	NOT_DIR,
};

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	char			**heredoc;
	char			**infile;
	char			**outfile;
	int				fd_infile;
	int				fd_outfile;
	int				*flag_redir;
	bool			here;
	bool			expand;
	bool			empty_var_cmd;
	char			*file;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_token
{
	char			*str;
	int				type;
	bool			expand;
	bool			empty_var_tok;
	struct s_token	*next;
}					t_token;

typedef struct s_lst
{
	char			*str;
	struct s_lst	*next;
}					t_lst;

typedef struct s_pid
{
	pid_t			cur_pid;
	struct s_pid	*next;
}					t_pid;

typedef struct s_data
{
	char			*pwd;
	char			*line;
	char			*shell_pid;
	char			**env_cp;
	int				fd[2];
	int				nb_cmd;
	int				original_stdin;
	int				original_stdout;
	int				gexit_code;
	t_token			*token;
	t_cmd			*cmd;
	t_lst			*env;
	t_list			*pid;
}					t_data;

typedef struct s_env_var
{
	char			*var;
	char			*next;
	char			*prev;
	int				*quote_tab;
	int				dollars;
	size_t			i;
	bool			stop;
	bool			heredoc;
}					t_env_var;

# define BUFFER_SIZE 1000
# define DOUBLE_QUOTE 34
# define SINGLE_QUOTE 39

// commands_redir.c
void				parse_heredoc(t_cmd *cur_cmd, t_token *cur_tok);
void				save_files(t_cmd *cur_cmd, t_token *cur_tok, int *i,
						int *j);
void				redir_cmd(t_cmd *cur_cmd, t_token *cur_tok);
void				get_flag_redir(t_data *data, t_cmd *cur_cmd,
						t_token *cur_tok);
void				manage_redirs(t_data *data, t_cmd *cur_cmd,
						t_token *cur_tok);

// commands_utils.c
int					is_built_in(char *str);
int					count_token(t_token *cur, int token);
int					count_args(t_token *tmp);

// commands.c
t_token				*get_args(t_cmd *cur_cmd, t_token *cur_tok);
void				save_cmd(t_data *data, t_cmd *cur_cmd, t_token *cur_tok);
t_token				*build_cmd(t_data *data, t_cmd *cur_cmd, t_token *cur_tok);
void				get_cmds(t_data *data);

// init.c
void				init_env(t_data *data, char **env);
void				init_data(t_data *data, char **env);

// parsing_utils.c
int					check_quotes(t_data *data, char *input);
void				get_shell_pid(t_data *data);
int					is_white_space(char c);

// parsing.c
int					check_syntax(t_data *data);
int					launch_heredoc(t_data *data, t_cmd *cur_cmd,
						t_token *cur_tok);
int					parsing(t_data *data);

// prompt.c
void				read_prompt(t_data *data);
void				prompt(t_data *data);

// token_utils.c
void				rebuild_cmd(t_data *data, char *str);
int					skip_in_quotes(char *str, int i);
char				*remove_quotes(t_data *data, char *str);
int					count_words(char *str);
int					is_separator_token(char c);

// token.c
int					get_token_len(char *str);
void				new_token_node(t_data *data, char *str, int len);
void				add_token(t_data *data, int i);
void				get_token(char *str, t_token *cur);
void				tokenise(t_data *data);

// env_variables.c
char				*env_variables(t_data *data, char *line, bool heredoc);
int					*expansion_quotes(char *line, int nb_var, bool heredoc);

// expand_line.c
char				*expand_line(t_data *data, char *line, t_env_var *info);
char				*is_var(t_data *data, char *line, t_env_var *info);

// env_variables_utils.c
char				*only_dollars(t_data *data, char *line, t_env_var *info);
int					is_separator_env(char c, int pos);
int					count_char(char *str, char c);
char				*before_expand(t_data *data, char *line, t_env_var *info);

#endif