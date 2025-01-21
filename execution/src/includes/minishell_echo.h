/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_echo.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:31 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/14 14:11:38 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ECHO_H
# define MINISHELL_ECHO_H

# include "minishell.h"

int		count_file(char **argv);

int		count_trailing_redirects(char **argv, int argc);
int		handle_options(char **argv, int *endl);
int		open_output_file(char **argv, int argc);
int		ft_echo(int argc, char **argv);

void	write_no_arguments(char **argv, int endl, int save, int fd);
int		ft_write_argv_space(char **argv, int j, int dif, int fd);
void	ft_write_argv_endl(char **argv, int j, int endl, int fd);
void	write_arguments(char **argv, int fd, int endl, int save);

#endif