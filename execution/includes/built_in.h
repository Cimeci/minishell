/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:52:27 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/21 15:12:41 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

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
void		ft_update_pwd(t_data *data, int info);
int			ft_cd_root(t_data *data, t_cmd *cur);
int			ft_cd(t_data *data, t_cmd *cur);

// update_pwd.c //

void		ft_update_pwd(t_data *data, int info);
void		ft_handle_pwd_update(t_data *data, t_lst *tmp, char *pwd,
				char *get_pwd);
void		ft_handle_oldpwd_update(t_data *data, char *get_oldpwd,
				char *get_pwd);
void		ft_update_env_vars(t_lst *tmp, char *pwd, char *var_name);

// // echo //

int			handle_options(char **argv, int *endl);
void		write_arguments(char **argv, int endl, int save);
int			ft_echo(char **argv);

// // env //

char		*tab_to_lower(char *str);
void		ft_sort_env(char **env, int size);
int			ft_env(t_data *data, t_cmd *cur);

// // export //

void		ft_modif_env_var(t_lst *cur, char *var, char *arg);
int			ft_export_append(t_data *data, char *arg);
int			ft_export_assign(t_data *data, char *var, char *value);
int			ft_export(t_data *data, t_cmd *cur);

// // // get //

char		*ft_get_var(char *str);
char		*ft_get_value(char *argv);
char		*ft_get_var_and_value(char *var, t_lst *env);
char		*ft_get_pvar(char *argv);

// // // check_print //

int			ft_check_env_var(t_data *data, char *var);
int			ft_check_isalnum(t_data *data, char *var, int i);

int			print_export(t_data *data, t_cmd *cur);

// // pwd //

char		*ft_find_pwd(t_data *data);
int			ft_pwd(t_data *data);

// // unset //

void		ft_unset_extension(char *unset_path, t_lst *env);
int			ft_unset(t_data *data, t_cmd *cur);

// // exit //

void		ft_exit(t_data *data, t_cmd *cur);

long long	ft_atoll(char *str, int *error);
int			check_max_min(long long nb, char *str, int inf);
char		*ft_remove_space(char *str);
int			ft_char_is_digit(char *str);
void		clean_error_exit(t_data *data, char *str1, char *str2, int info);

// // lst_funct_utils.c //

int			ft_add_to_list(t_lst **list, char *arg);
void		*ft_lstnew_generic(size_t data_size);
void		ft_lstadd_back_generic(void **lst, void *new_node,
				size_t next_offset);
int			ft_lstsize_generic(void *lst, size_t offset);
void		*ft_lstlast_generic(void *lst, size_t offset);

// // lst_funct.c //
char		**ft_convert_lst_to_tab(t_lst *env);
char		*my_getenv_lst(const char *name, t_lst *env);

// // exec.c //
int			setup_execution(t_data *data);
void		execute_pipeline(t_data *data);
void		exec(t_data *data);
void		child(t_data *data, t_cmd *cur, int i, bool prev_cmd);
int			is_built_in(char *str);
int			handle_unique_builtin(t_data *data, t_cmd *cur);
int			exec_built_in(t_data *data, t_cmd *cur);
void		close_files(t_cmd *cur, t_data *data, int fd, bool child);
bool		open_redir(t_data *data, t_cmd *cur, int type, int i);
int			files(t_data *data, t_cmd *cur);
int			handle_here_doc(t_data *data, t_cmd *cur);
void		handle_commande_execution(t_data *data, t_cmd *cur);
void		check_exec_cmd(t_data *data, t_cmd *cur);
void		parent(t_data *data);
void		handle_parent_process(t_data *data, t_cmd *last);
void		cleanup_execution(t_data *data);
void		setup_child_process(t_data *data, t_cmd *cur);

// // heredoc //

void		ft_heredoc(t_data *data, t_cmd *cur);

// write_heredoc.c //

// void		write_tmpfile(t_data *data, t_cmd *cur, int fd);
// void		process_heredoc_input(t_data *data, t_cmd *cur, int fd,
// 				int original_in);
// char		*read_heredoc_line(t_data *data, char *delimiter, int original_in);
// void		extension_write(t_data *data, t_cmd *cur, char *input, int fd);

// // signal.c //

void		child_signal_handler(int signum);
void		parent_signal_handler(int signum);
void		parent_signal_handler_exec(int signum);
void		parent_signal_handler_here(int signum);

// // randomizer.c //

char		*randomizer(void);

#endif