/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstclear_ft.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:14:10 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/10 19:23:20 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "malloc.h"

void	lstclear_ft(t_list **lst, void (*del)(void*))
{
	t_list	*tmp;

	tmp = *lst;
	while ((*lst)->next)
	{
		tmp = (*lst)->next;
		(*del)((*lst)->content);
		free(*lst);
		*lst = tmp;
	}
	(*del)((*lst)->content);
	free(*lst);
	*lst = NULL;
}
