/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strjoin_ft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:21:33 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/10 19:18:18 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*strjoin_ft(char const *s1, char const *s2)
{
	char	*dst;
	char	*tmp;

	dst = malloc(strlen_ft(s1) + strlen_ft(s2) + 1);
	if (!dst)
		return (NULL);
	tmp = dst;
	while (*s1)
		*tmp++ = *s1++;
	while (*s2)
		*tmp++ = *s2++;
	*tmp = 0;
	return (dst);
}
