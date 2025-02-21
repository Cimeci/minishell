/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:52:27 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/21 16:42:39 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

typedef struct s_file
{
	int		fd_o;
	int		fd_i;
	int		type;
	bool	error;
}			t_file;

typedef struct s_tmp_file
{
	int		fd;
	char	*input;
	int		original_in;
}			t_tmp_file;

// cd //

void		ft_absolut_path(t_data *data, t_cmd *cur);
void		ft_update_extension(t_data *data, t_lst *tmp, char *var);
int			ft_cd_root(t_data *data, t_cmd *cur);
int			ft_cd(t_data *data, t_cmd *cur);

// // update_pwd.c // //

void		ft_update_pwd(t_data *data, int info);
void		ft_handle_pwd_update(t_data *data, t_lst *tmp, char *pwd,
				char *get_pwd);
void		ft_handle_oldpwd_update(t_data *data, char *get_oldpwd,
				char *get_pwd);
void		ft_update_env_vars(t_lst *tmp, char *pwd, char *var_name);

// echo //

int			handle_options(char **argv, int *endl);
void		write_arguments(char **argv, int endl, int save);
int			ft_echo(char **argv);

// env //

int			ft_env(t_data *data, t_cmd *cur);

// export //

int			ft_export(t_data *data, t_cmd *cur);

// // export_handle //

void		ft_modif_env_var(t_lst *cur, char *var, char *arg);
int			ft_export_append(t_data *data, char *arg);
int			ft_export_assign(t_data *data, char *var, char *value);

// // get //

char		*ft_get_var(char *str);
char		*ft_get_pvar(char *argv);
char		*ft_get_var_and_value(char *var, t_lst *env);
char		*ft_get_value(char *argv);

// // check_print //

int			ft_check_isalnum(t_data *data, char *var, int i);
int			ft_check_env_var(t_data *data, char *var);

// pwd //

char		*ft_find_pwd(t_data *data);
int			ft_pwd(t_data *data);

// unset //

void		ft_unset_extension(char *unset_path, t_lst *env);
int			ft_unset(t_data *data, t_cmd *cur);

// exit //

void		ft_exit(t_data *data, t_cmd *cur);

// // exit_utils.c //

long long	ft_atoll(char *str, int *error);
int			check_max_min(long long nb, char *str, int inf);
char		*ft_remove_space(char *str);
int			ft_char_is_digit(char *str);
void		clean_error_exit(t_data *data, char *str1, char *str2, int info);

// exec.c //

int			setup_execution(t_data *data);
void		execute_pipeline(t_data *data);
void		exec(t_data *data);

// exec_built_in.c //

int			exec_built_in(t_data *data, t_cmd *cur);
int			handle_unique_builtin(t_data *data, t_cmd *cur);

// exec_child.c //

void		child(t_data *data, t_cmd *cur, int i, bool prev_cmd);

// exec_file.c //

void		close_files(t_cmd *cur, t_data *data, int fd, bool child);
bool		open_redir(t_data *data, t_cmd *cur, int type, int i);

// exec_handle_file.c //

int			files(t_data *data, t_cmd *cur);

// exec_handle.c //

void		handle_commande_execution(t_data *data, t_cmd *cur);

// exec_parent.c //

void		parent(t_data *data);
void		handle_parent_process(t_data *data, t_cmd *last);
void		cleanup_execution(t_data *data);

// heredoc //

void		ft_heredoc(t_data *data, t_cmd *cur);

// randomizer.c //

char		*randomizer(void);

// signal.c //

void		child_signal_handler(int signum);
void		parent_signal_handler_exec(int signum);
void		parent_signal_handler(int signum);

#endif