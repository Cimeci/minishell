/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:15:22 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 15:07:03 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <term.h>
# include <errno.h>

typedef struct s_env
{
	char			*path;
	struct s_env	*next;
}					t_env;

# include "minishell_cd.h"
# include "minishell_echo.h"
# include "minishell_env.h"
# include "minishell_exit.h"
# include "minishell_export.h"
# include "minishell_pwd.h"
# include "minishell_unset.h"

void	execution_cmd(char **argv, char **env);

#endif