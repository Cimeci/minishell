/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:52:27 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/28 10:26:41 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "minishell.h"

# ifndef G_PID
extern pid_t	g_pid;
# endif

// cd //

void			ft_free_path(char *path, char *target_path);
char			*ft_strsstr(char *pwd, char *cmd);
void			ft_absolut_path(t_data *data, t_cmd *cur);
int				ft_cd(t_data *data, t_cmd *cur);

// // echo //

int				count_file(char **argv);

int				count_trailing_redirects(char **argv, int argc);
int				handle_options(char **argv, int *endl);
int				open_output_file(char **argv, int argc);
int				ft_echo(char **argv);

void			write_no_arguments(char **argv, int endl, int save);
int				ft_write_argv_space(char **argv, int j, int dif);
void			ft_write_argv_endl(char **argv, int j, int endl);
void			write_arguments(char **argv, int endl, int save);

// // env //

char			*tab_to_lower(char *str);
void			ft_sort_env(char **env, int size);
int				ft_env(t_data *data, t_cmd *cur);

// // export //

char			*ft_get_var(char *str);
void			ft_modif_env_var(t_lst *cur, t_lst *env, char *var, char *arg);
int				ft_export(t_data *data, t_cmd *cur);
char			*ft_get_value(char *argv);
char			*ft_get_var_and_value(char *var, t_lst *env);
char			*ft_get_pvar(char *argv);
int				ft_check_env_var(char *var);
char			*my_getenv_lst(const char *name, t_lst *env);
void			ft_print_env_export(t_lst *env, char **argv);

int				ft_handle_existing_update(t_data *data, char *arg);
int				ft_handle_new_var_addition(t_data *data, char *arg);
int				ft_add_to_list(t_lst **list, char *arg);
void			ft_print_export_env(t_lst *export_env);
int				ft_export(t_data *data, t_cmd *cur);

// // pwd //

char			*ft_find_pwd(void);
int				ft_pwd(t_data *data);

// // unset //

void			ft_unset_extension(char *unset_path, t_lst *env);
int				ft_unset(t_data *data, t_cmd *cur);

// // exit //

// void	ft_exit(char **argv);

// // lst_funct_utils.c //

void			*ft_lstnew_generic(size_t data_size);
void			ft_lstadd_back_generic(void **lst, void *new_node,
					size_t next_offset);
int				ft_lstsize_generic(void *lst, size_t offset);

// // lst_funct.c //
char			**ft_convert_lst_to_tab(t_lst *env);
t_lst			*ft_dup_lst(t_lst *env);
char			*my_getenv_lst(const char *name, t_lst *env);

// // exec.c //
void			exec(t_data *data);
int				is_built_in(t_data *data, t_cmd *cur);

// // heredoc //

void			ft_heredoc(t_data *data, t_cmd *cur);

// // signal.c //

void			child_signal_handler(int signum);
void			parent_signal_handler(int signum);

// // randomizer.c //

char			*randomizer(void);

#endif