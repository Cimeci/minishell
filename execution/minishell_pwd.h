/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pwd.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inowak-- <inowak--@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:10:04 by inowak--          #+#    #+#             */
/*   Updated: 2025/01/14 15:00:25 by inowak--         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_PWD_H
# define MINISHELL_PWD_H

# include "minishell.h"

char	*ft_clear_quote(char *input);
int		ft_pwd(char **argv);

#endif