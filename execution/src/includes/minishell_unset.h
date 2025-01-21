/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_unset.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:16:47 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/15 11:29:03 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_UNSET_H
# define MINISHELL_UNSET_H

# include "minishell.h"

char	*get_path(const char *name, t_env *env);
void	ft_unset_extension(char *unset_path, t_env *env);
int		ft_unset(char **argv, t_env *env, t_env *export_env);

t_env	*ft_init_env(char **env);
char	*my_getenv_lst(const char *name, t_env *env);

#endif