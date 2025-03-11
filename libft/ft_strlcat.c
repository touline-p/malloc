/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strlcat_ft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 23:57:35 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/11 13:03:36 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	strlcat_ft(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	k;
	size_t	len_base;

	i = 0;
	k = 0;
	while (i < size && dst[i])
		i++;
	if (i + 1 > size)
		return (size + strlen_ft(src));
	len_base = i;
	while (size > i + 1 && k < strlen_ft(src))
		dst[i++] = src [k++];
	if (i < size)
		dst[i] = 0;
	return (len_base + strlen_ft(src));
}
