/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncharbog <ncharbog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 11:59:19 by ncharbog          #+#    #+#             */
/*   Updated: 2024/10/17 10:35:19 by ncharbog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(int n)
{
	size_t		count;

	count = 1;
	if (n < 0)
		n = n * -1;
	while (n >= 10)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static char	*ft_negative(int n, size_t count)
{
	char	*dest;

	count = count + 2;
	dest = malloc (count * sizeof (char));
	if (dest == NULL)
		return (0);
	n = n * -1;
	count--;
	dest[count] = '\0';
	count--;
	while (n >= 10)
	{
		dest[count] = n % 10 + '0';
		n = n / 10;
		count--;
	}
	dest[count] = n % 10 + '0';
	dest[count - 1] = '-';
	return (dest);
}

static char	*ft_positive(int n, size_t count)
{
	char	*dest;

	count = count + 1;
	dest = malloc (count * sizeof (char));
	if (dest == NULL)
		return (0);
	count--;
	dest[count] = '\0';
	count--;
	while (n >= 10)
	{
		dest[count] = n % 10 + '0';
		n = n / 10;
		count--;
	}
	dest[count] = n % 10 + '0';
	return (dest);
}

char	*ft_itoa(int n)
{
	char		*dest;
	size_t		count;

	count = ft_count(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n < 0)
		dest = ft_negative(n, count);
	if (n >= 0)
		dest = ft_positive(n, count);
	return (dest);
}
