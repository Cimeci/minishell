/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:52:27 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/14 14:54:03 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "minishell.h"

// cd //

void	ft_absolut_path(t_data *data, t_cmd *cur);
void	ft_update_extension(t_data *data, t_lst *tmp, char *var);
void	ft_update_pwd(t_data *data, int info);
int		ft_cd_root(t_data *data, t_cmd *cur);
int		ft_cd(t_data *data, t_cmd *cur);

// // echo //

int		handle_options(char **argv, int *endl);
void	write_arguments(char **argv, int endl, int save);
int		ft_echo(char **argv);

// // env //

char	*tab_to_lower(char *str);
void	ft_sort_env(char **env, int size);
int		ft_env(t_data *data, t_cmd *cur);

// // export //

void	ft_modif_env_var(t_lst *cur, t_lst *env, char *var, char *arg);
int		ft_export_append(t_data *data, char *arg);
int		ft_export_assign(t_data *data, char *var, char *value);
int		ft_export(t_data *data, t_cmd *cur);

// // // get //

char	*ft_get_var(char *str);
char	*ft_get_value(char *argv);
char	*ft_get_var_and_value(char *var, t_lst *env);
char	*ft_get_pvar(char *argv);

// // // check_print //

int		ft_check_env_var(t_data *data, char *var);
int		ft_check_isalnum(t_data *data, char *var, int i);

void	ft_print_export_env(t_lst *export_env);
int		print_export(t_data *data, t_cmd *cur);

// // pwd //

char	*ft_find_pwd(t_data *data);
int		ft_pwd(t_data *data);

// // unset //

void	ft_unset_extension(char *unset_path, t_lst *env);
int		ft_unset(t_data *data, t_cmd *cur);

// // exit //

void	ft_exit(t_data *data, t_cmd *cur);

// // lst_funct_utils.c //

int		ft_add_to_list(t_lst **list, char *arg);
void	*ft_lstnew_generic(size_t data_size);
void	ft_lstadd_back_generic(void **lst, void *new_node, size_t next_offset);
int		ft_lstsize_generic(void *lst, size_t offset);
void	*ft_lstlast_generic(void *lst, size_t offset);

// // lst_funct.c //
char	**ft_convert_lst_to_tab(t_lst *env);
t_lst	*ft_dup_lst(t_lst *env);
char	*my_getenv_lst(const char *name, t_lst *env);

// // exec.c //
int		setup_execution(t_data *data);
void	execute_pipeline(t_data *data);
void	exec(t_data *data);
void	child(t_data *data, t_cmd *cur, int i);
int		is_built_in(char *str);
int		handle_unique_builtin(t_data *data, t_cmd *cur);
int		exec_built_in(t_data *data, t_cmd *cur);
void	close_files(t_cmd *cur, t_data *data, int fd);
void	open_redir(t_cmd *cur, int type, int i);
void	files(t_data *data, t_cmd *cur);
int		handle_here_doc(t_data *data, t_cmd *cur);
void	handle_commande_execution(t_data *data, t_cmd *cur);
void	check_exec_cmd(t_data *data, t_cmd *cur);
void	parent(t_data *data);
void	handle_parent_process(t_data *data);
void	cleanup_execution(t_data *data);

// // heredoc //

void	ft_heredoc(t_data *data, t_cmd *cur);

// // signal.c //

void	child_signal_handler(int signum);
void	parent_signal_handler(int signum);
void	parent_signal_handler_exec(int signum);
void	parent_signal_handler_here(int signum);

// // randomizer.c //

char	*randomizer(void);

#endif