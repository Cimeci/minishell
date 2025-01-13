/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:36:50 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 14:53:27 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_EXPORT_H
# define MINISHELL_EXPORT_H

# include "minishell.h"

char	*ft_get_var(char *str);
void	ft_modif_env_var(t_env *cur, t_env *env, char *var, char *arg);
int		ft_export(char **argv, t_env *env);

t_env	*ft_init_env(char **env);
char	*my_getenv_lst(const char *name, t_env *env);

#endif