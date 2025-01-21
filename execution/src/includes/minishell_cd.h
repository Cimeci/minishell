/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cd.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:07:50 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/14 14:11:34 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_CD_H
# define MINISHELL_CD_H

# include "minishell.h"

void	ft_free_path(char *path, char *target_path);
void	ft_free_tab(char **table);
char	*ft_strsstr(char *pwd, char *cmd);
void	ft_absolut_path(char **argv, char *root);
int		ft_cd(char **argv);
int		main_cd(int argc, char **argv, char **env);

#endif