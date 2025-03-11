/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_ft.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:16:50 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/08 19:00:39 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "malloc.h"

static int	len_ft(int n, int *sign)
{
	int	ln;

	ln = 2;
	if (n < 0)
	{
		*sign = -1;
		ln++;
		while (n < -9)
		{
			ln++;
			n /= 10;
		}
	}
	else
	{
		*sign = 1;
		while (n > 9)
		{
			ln++;
			n /= 10;
		}
	}
	return (ln);
}

char	*itoa_ft(int n)
{
	char	*dst;
	char	*pin;
	int		size;
	int		sign;

	sign = 1;
	size = len_ft(n, &sign);
	dst = (char *)malloc(size);
	if (!dst)
		return (NULL);
	pin = dst + size - 1;
	*(pin--) = 0;
	while (1)
	{
		*pin = (n % 10 * sign) + '0';
		if (pin == dst)
			break ;
		pin--;
		n /= 10;
	}
	if (sign == -1)
		*pin = '-';
	return (dst);
}
