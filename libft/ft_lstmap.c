/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstmap_ft.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:19:13 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/09 17:14:44 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "malloc.h"

t_list	*lstmap_ft(t_list *lst, void *(*f)(void*), void(*del)(void *))
{
	void	*data;
	t_list	*new;
	t_list	*pin;

	data = (*f)(lst->content);
	new = lstnew_ft(data);
	if (!new)
	{
		free(data);
		return (NULL);
	}
	pin = new;
	while (lst->next)
	{
		lst = lst->next;
		data = (*f)(lst->content);
		pin->next = lstnew_ft(data);
		if (!pin->next)
		{
			free(data);
			lstclear_ft(&new, del);
			return (NULL);
		}
		pin = pin->next;
	}
	return (new);
}
