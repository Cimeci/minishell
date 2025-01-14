/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:15:22 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/14 15:31:23 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_env
{
	char			*path;
	struct s_env	*next;
}					t_env;

# include "../libft/libft.h"
# include "minishell_cd.h"
# include "minishell_echo.h"
# include "minishell_env.h"
# include "minishell_exit.h"
# include "minishell_export.h"
# include "minishell_pwd.h"
# include "minishell_unset.h"
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

void				execution_cmd(char **argv, char **env);
int					ft_strlen_tab(char **table);

#endif