/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strchr_ft.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:21:13 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/09 15:03:19 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*strchr_ft(const char *s, int c)
{
	char	*tmp;

	tmp = (char *)s;
	while (1)
	{
		if (*tmp == (unsigned char)c)
			return (tmp);
		if (!*tmp)
			return (NULL);
		tmp++;
	}
	return (NULL);
}
