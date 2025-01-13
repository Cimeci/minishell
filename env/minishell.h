/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:36:50 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/13 10:54:12 by inowak--         ###   ########.fr       */
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

typedef struct s_env
{
	char			*path;
	struct s_env	*next;
}					t_env;

// command.c //

char				*my_getenv(const char *name, char **env);
char				*find_pathname(char *cmd, char **cmd_split,
						char **path_split, int i);
char				*find_path(char **env, char *cmd);
void				execution_cmd(char **argv, char **env);

// lst_funct.c //

void				ft_lstclear2(t_env *env);
void				ft_lstadd_back2(t_env **lst, t_env *new);
int					ft_lstsize2(t_env *lst);
t_env				*ft_lstnew2(void *path);
char				**ft_convert_lst_to_tab(t_env *env);

// main.c

void				ft_free_tab(char **table);
t_env				*ft_init_env(char **env);
t_env				*ft_env(char **argv, t_env *env);
char				*use_input(char *input, t_env *list, char **argv);

#endif