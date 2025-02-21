/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:54:05 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/21 16:38:46 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

// errors.c
void	print_errors(char *str, ...);
int		errors(t_data *data, char *str, int type);
void	errors_exec(char *msg, char *cmd, int type);

// free_lst.c
void	free_token(t_token **head);
void	free_env(t_lst **head);
void	free_cmd(t_cmd **head);

// free.c
void	ft_free_tab(char **table);
void	free_all(t_data *data, int flag);
char	*ft_strjoin_free(char *s1, char *s2, int flag);

// utils_libft.c
int		ft_strlen_tab(char **table);
char	*ft_strsstr(char *pwd, char *cmd);
int		ft_strcmp(char *s1, char *s2);

// utils_path.c
char	*my_getenv(t_data *data, char *name);
char	*extend_find(char *str, char **path_split, int i);
char	*find_path(t_data *data, char *str);

// lst_funct_utils.c //

int			ft_add_to_list(t_lst **list, char *arg);
void		*ft_lstnew_generic(size_t data_size);
void		ft_lstadd_back_generic(void **lst, void *new_node,
				size_t next_offset);
int			ft_lstsize_generic(void *lst, size_t offset);
void		*ft_lstlast_generic(void *lst, size_t offset);

// lst_funct.c //

char		**ft_convert_lst_to_tab(t_lst *env);
char		*my_getenv_lst(const char *name, t_lst *env);

#endif