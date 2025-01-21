/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:37:10 by ncharbog          #+#    #+#             */
/*   Updated: 2024/10/16 11:06:39 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check(char const *set, char const c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	ft_start(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (s1[i] && ft_check(set, s1[i]) > 0)
		i++;
	return (i);
}

static int	ft_end(char const *s1, char const *set)
{
	int	i;

	i = ft_strlen(s1);
	i--;
	while (i > 0 && ft_check(set, s1[i]) > 0)
		i--;
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		len;
	int		start;
	int		end;
	int		i;
	char	*dest;

	if (!s1 || !set)
		return (0);
	start = ft_start(s1, set);
	end = ft_end(s1, set) + 1;
	len = end - start;
	if (len <= 0)
		return (ft_strdup(""));
	dest = malloc((len + 1) * sizeof(char));
	if (dest == NULL)
		return (0);
	i = 0;
	while (i < len)
	{
		dest[i] = s1[start];
		i++;
		start++;
	}
	dest[i] = '\0';
	return (dest);
}
