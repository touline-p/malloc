/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 22:37:16 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/22 22:39:05 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "printf_ft.h"
#include <string.h>
#include <stdarg.h>



int	sub_printf_ft(int fd, const char *format, va_list params) {
	size_t used_len;
	size_t len_to_copy;
	char *ptr;
	print_specifiers_t spec = {};

	used_len = 0;
	while (*format)
	{
		if (*format != '%') {
			spec.string = (char *)format;	
			ptr = strchr(format, '%');
			if (ptr) {
				spec.len = ptr - format;
			} else {
				spec.len = strlen(format);
			}
			print_from_spec(&spec);
			format += spec.len;
		}
		else {
			format = ft_print_param(format, params);
		}
	}
	va_end(params);
	spec.string = NULL;
	return true;
}

int printf_ft(const char *format, ...) {
	va_list params;

	write(1, "$", 1);
	va_start(params, format);
	return sub_printf_ft(1, format, params);
}

