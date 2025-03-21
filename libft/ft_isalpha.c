/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isalpha_ft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:14:18 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/08 13:33:23 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

int	isalpha_ft(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1024);
	return (0);
}
