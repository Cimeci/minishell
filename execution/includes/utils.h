/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:54:05 by ncharbog          #+#    #+#             */
/*   Updated: 2025/02/20 18:58:57 by ncharbog         ###   ########.fr       */
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

#endif