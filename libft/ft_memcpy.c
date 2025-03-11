/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memcpy_ft.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:19:52 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/08 18:48:28 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"

void	*memcpy_ft(void *dst, const void *src, size_t length)
{
	char	*tmp;

	if (dst == src)
		return (dst);
	tmp = dst;
	while (length--)
	{
		*tmp = *((char *)src);
		tmp++;
		src++;
	}
	return (dst);
}
