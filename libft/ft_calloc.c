/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc_ft.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:20:14 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/09 12:36:57 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 
#include "limits.h"
#include "malloc.h"

void	*calloc_ft(size_t n, size_t size)
{
	void	*ptr;

	if (size == 0)
		return (strdup_ft(""));
	if (n > UINT_MAX / size)
		return (NULL);
	ptr = (void *)malloc(n * size);
	if (!ptr)
		return (NULL);
	bzero_ft(ptr, n * size);
	return (ptr);
}
