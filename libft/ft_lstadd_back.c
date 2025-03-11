/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstadd_back_ft.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:36:05 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/09 19:34:02 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	lstadd_back_ft(t_list **lst, t_list *new)
{
	t_list	*pin;

	if (!lst)
		return ;
	if (*lst)
	{
		pin = lstlast_ft(*lst);
		if (pin)
			pin->next = new;
	}
	else
		(*lst) = new;
}
