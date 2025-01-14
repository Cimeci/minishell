/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:24:30 by ncharbog          #+#    #+#             */
/*   Updated: 2024/10/16 10:37:54 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if (c >= '0' && c <= '9')
		return (8);
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (8);
	return (0);
}
