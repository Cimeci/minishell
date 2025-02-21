/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:02:48 by ncharbog          #+#    #+#             */
/*   Updated: 2024/10/17 08:14:33 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*char_dest;
	unsigned char		*char_src;

	if (dest == NULL && src == NULL)
		return (0);
	if (dest <= src)
		return (ft_memcpy(dest, src, n));
	char_dest = (unsigned char *)dest + n - 1;
	char_src = (unsigned char *)src + n - 1;
	while (n)
	{
		*char_dest = *char_src;
		char_dest--;
		char_src--;
		n--;
	}
	return (dest);
}
