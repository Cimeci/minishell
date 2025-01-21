/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:52:27 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/21 16:53:27 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "minishell.h"

// cd //

void	ft_free_path(char *path, char *target_path);
void	ft_free_tab(char **table);
char	*ft_strsstr(char *pwd, char *cmd);
void	ft_absolut_path(char **argv, char *root);
int		ft_cd(char **argv);
int		main_cd(int argc, char **argv, char **env);

// // echo //

int		count_file(char **argv);

int		count_trailing_redirects(char **argv, int argc);
int		handle_options(char **argv, int *endl);
int		open_output_file(char **argv, int argc);
int		ft_echo(int argc, char **argv);

void	write_no_arguments(char **argv, int endl, int save, int fd);
int		ft_write_argv_space(char **argv, int j, int dif, int fd);
void	ft_write_argv_endl(char **argv, int j, int endl, int fd);
void	write_arguments(char **argv, int fd, int endl, int save);

// env //

// // command.c

// char	*my_getenv(const char *name, char **env);
// char	*find_pathname(char *cmd, char **cmd_split, char **path_split, int i);
// char	*find_path(char **env, char *cmd);

// lst_funct.c

void	ft_lstclear2(t_lst *env);
void	ft_lstadd_back2(t_lst **lst, t_lst *new);
int		ft_lstsize2(t_lst *lst);
t_lst	*ft_lstnew2(void *path);
char	**ft_convert_lst_to_tab(t_lst *env);

// // main.c

// t_env	*ft_init_env(char **env);
int	ft_env(t_data *data, t_cmd *cur);

// // exit //

// void	ft_exit(char **argv);

// // export //

char	*ft_get_var(char *str);
void	ft_modif_env_var(t_lst *cur, t_lst *env, char *var_line);
int		ft_export(t_data *data, t_cmd *cur);

// t_lst	*ft_init_env(char **env);
char	*my_getenv_lst(const char *name, t_lst *env);

// pwd //

int		ft_pwd(void);

// // unset //

// char	*get_path(const char *name, t_env *env);
// void	ft_unset_extension(char *unset_path, t_env *env);
// int		ft_unset(char **argv, t_env *env, t_env *export_env);

// t_env	*ft_init_env(char **env);
// char	*my_getenv_lst(const char *name, t_env *env);


#endif