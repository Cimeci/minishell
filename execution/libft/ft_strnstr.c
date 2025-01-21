/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:42:55 by ncharbog          #+#    #+#             */
/*   Updated: 2024/10/16 18:55:37 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	p;

	i = 0;
	p = 0;
	if ((!big || !little) && len == 0)
		return (0);
	if (little[0] == '\0')
		return ((char *)big);
	while (big[i] != '\0' && i < len)
	{
		while (big[i + p] == little[p] && i + p < len)
		{
			p++;
			if (little[p] == '\0')
				return ((char *)big + i);
		}
		p = 0;
		i++;
	}
	return (0);
}
