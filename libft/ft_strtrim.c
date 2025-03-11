/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtrim_ft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:22:16 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/10 19:19:16 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_from_ft(char c, char *charset)
{
	while (*charset)
	{
		if (c == *charset)
			return (1);
		charset++;
	}
	return (0);
}

char	*strtrim_ft(char const *s1, char const *set)
{
	char	*start;
	char	*pin;
	char	*dst;
	size_t	ln;

	pin = (char *)s1;
	while (is_from_ft(*pin, (char *)set))
		pin++;
	if (!*pin)
		return (strdup_ft(""));
	start = pin;
	while (*pin)
		pin++;
	while (is_from_ft(*(pin - 1), (char *)set))
		pin--;
	ln = pin - start + 1;
	dst = malloc(ln);
	if (!dst)
		return (NULL);
	strlcpy_ft(dst, start, ln);
	return (dst);
}
