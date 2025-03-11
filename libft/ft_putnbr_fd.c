/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putnbr_fd_ft.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:20:39 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/08 13:50:03 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"

static	void	putnbr_ft(int n, int fd)
{
	char	c;

	if (n > 9)
		putnbr_ft(n / 10, fd);
	c = ('0' + n % 10);
	write(fd, &c, 1);
}

void	putnbr_fd_ft(int n, int fd)
{
	int		sign;
	char	c;

	sign = 1;
	if (n < 0)
	{
		write(fd, "-", 1);
		sign = -1;
	}
	if (n > 9 || n < -9)
		putnbr_ft(n / (10 * sign), fd);
	c = ('0' + n % 10 * sign);
	write(fd, &c, 1);
}
