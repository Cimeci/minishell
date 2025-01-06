/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:00:37 by ncharbog          #+#    #+#             */
/*   Updated: 2024/10/16 10:44:39 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*char_s1;
	unsigned char	*char_s2;

	char_s1 = (unsigned char *)s1;
	char_s2 = (unsigned char *)s2;
	while (n)
	{
		if (*char_s1 != *char_s2)
			return (*char_s1 - *char_s2);
		char_s1++;
		char_s2++;
		n--;
	}
	return (0);
}
