/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_env.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:36:50 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 15:06:53 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ENV_H
# define MINISHELL_ENV_H

# include "minishell.h"

// command.c //

char	*my_getenv(const char *name, char **env);
char	*find_pathname(char *cmd, char **cmd_split, char **path_split, int i);
char	*find_path(char **env, char *cmd);

// lst_funct.c //

void	ft_lstclear2(t_env *env);
void	ft_lstadd_back2(t_env **lst, t_env *new);
int		ft_lstsize2(t_env *lst);
t_env	*ft_lstnew2(void *path);
char	**ft_convert_lst_to_tab(t_env *env);

// main.c

t_env	*ft_init_env(char **env);
int		ft_env(char **argv, t_env *env);

#endif