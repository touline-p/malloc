/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strrchr_ft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:22:05 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/09 13:48:13 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*strrchr_ft(const char *s, int c)
{
	char	*tmp;
	char	*pin;

	if ((unsigned char)c == 0)
		return ((char *)(s + strlen_ft(s)));
	pin = NULL;
	tmp = strchr_ft(s, c);
	while (tmp != NULL)
	{
		pin = (char *)tmp;
		tmp = strchr_ft(tmp + 1, c);
	}
	return (pin);
}
