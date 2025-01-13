/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:31 by ncharbog          #+#    #+#             */
/*   Updated: 2025/01/13 10:35:35 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

void	ft_free_tab(char **table);
int		count_file(char **argv);
void	execution_cmd(char **argv, char **env);

int		count_trailing_redirects(char **argv, int argc);

void	write_no_arguments(char **argv, int endl, int save, int fd);
void	ft_write_argv_space(char **argv, int j, int dif, int fd);
void	ft_write_argv_endl(char **argv, int j, int endl, int fd);
void	write_arguments(char **argv, int fd, int endl, int save);

#endif