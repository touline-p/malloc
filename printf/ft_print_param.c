/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_param.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 23:48:02 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/13 23:48:13 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>

#include "printf_ft.h"

char *parse_flag_in_spec(print_specifiers_t *specs, char *format) {
	char *ptr;
	const char *flags = PRINTF_FLAGS;

	ptr = strchr(flags, *format);
	while (ptr) {
		specs->flags[ptr - flags] = true;
		format++;
		ptr = strchr(flags, *format);
	}
	return format;
}

char *parse_min_field_in_spec(print_specifiers_t *specs, char *format) {
	if ('0' < *format && *format < '9') {
		specs->min_field_width = strtol(format, &format, 10);
	}
	return format;
}

char *parse_precision_in_spec(print_specifiers_t *specs, char *format) {
	if (*format == '.') {
		specs->precision = strtol(format + 1, &format, 10);
	}
	return format;
}

void create_str_int(print_specifiers_t *specs, va_list param, char *buff);
void create_str_octal(print_specifiers_t *specs, va_list param, char *buff);
void create_str_hexa_min(print_specifiers_t *specs, va_list param, char *buff);
void create_str_hexa_maj(print_specifiers_t *specs, va_list param, char *buff);
void create_str_unsigned(print_specifiers_t *specs, va_list param, char *buff);
void create_str_character(print_specifiers_t *specs, va_list param, char *buff);
void create_str_string(print_specifiers_t *specs, va_list param, char *buff);
void create_str_pointer(print_specifiers_t *specs, va_list param, char *buff);
void create_str_percent(print_specifiers_t *specs, va_list param, char *buff);
void create_str_bad_identifiers(print_specifiers_t *specs, va_list param, char *buff);

char *parse_function_in_spec(print_specifiers_t *specs, char *format) {
	static const char *format_identifiers = FORMAT_IDENTIFIERS;	
	static create_str_function_t *fn_arr[FRMT_NB] = {
		&create_str_int, &create_str_int, &create_str_octal,
		&create_str_hexa_min, &create_str_hexa_maj,
		&create_str_unsigned, &create_str_character,
		&create_str_string, &create_str_pointer,
		&create_str_percent
	};

	char *ptr = strchr(format_identifiers, *format);
	if (NULL == ptr) {
		specs->function_to_use =  &create_str_bad_identifiers;
		specs->identifiers = BAD_FORMAT;
	} else {
		specs->identifiers = ptr - format_identifiers;
		specs->function_to_use = fn_arr[specs->identifiers];
	}
	return (++format);
}

char *itoa_base(long long nb, char *base, char *dst) {
	u_char size_base = strlen(base);
	char buffer[32] = {};
	char * ptr = dst;
	size_t index = 0;

	if (nb < 0) {
		nb *= -1;
		*dst = '-';
		++dst;
	}
	do {
		buffer[index++] = base[nb % size_base];
		nb = nb / size_base;
	} while (nb);
	ptr = dst;
	do {
		*ptr = buffer[--index];
		ptr++;
	} while (index);
	return dst;
}

#define BASE_8 "01234567"
#define BASE_10 "0123456789"
#define BASE_HEX_MIN "0123456789abcdef"
#define BASE_HEX_MAJ "0123456789ABCDEF"
#define DOT_SPACE 1
#define FLAGS_SPACE 3
#define ENDING_ZERO 1
#define NO_HASHTAG_STRING NULL

size_t len_of_nb(long long nb, size_t base) {
	size_t iterations;

	iterations = 0;

	while (nb) {
		iterations++;
		nb /= base;
	}
	return iterations;
}

void write_prefix_in_buff(int nb, char *hash_pref, char *to_fill, bool *flags) {
	char c[2] = {};
	if (hash_pref && flags[HASHTAG]) {
		strcpy(to_fill, hash_pref);
	}
	if (nb >= 0) {
		*c = flags[BLANK] * ' '
			+ (flags[BLANK] == false) * flags[PLUS_DEFAULT] * '+';
		strcat(to_fill, c);
	}
}

size_t max(size_t a, size_t b) {
	return a * (a > b) + b * (b > a);
}

void *build_string(print_specifiers_t *specs, size_t supposed_len, char *flag_buff, long long to_print) {
	size_t malloc_size;
	char *ret_val;
	size_t index_obj;
	static char buffer[256];

	bzero(buffer, 256);

	malloc_size = max(supposed_len, specs->min_field_width) + 1;
	ret_val = buffer;
	ret_val[malloc_size - 1] = '\0';
	if (specs->flags[LEFT_JUSTIFY]) {
		memset(ret_val, ' ', malloc_size - 1);
		memcpy(ret_val, flag_buff, strlen(flag_buff));
		itoa_base(to_print, BASE_10, ret_val + strlen(flag_buff));
		return ret_val;
	}
	index_obj = malloc_size - supposed_len - 1;
	if (specs->flags[ZERO_PADDING]) {
		memset(ret_val, '0', malloc_size - 1);
	}
	else {
		memset(ret_val, ' ', malloc_size - 1);
	}
	memcpy(ret_val + index_obj, flag_buff, strlen(flag_buff));
	itoa_base(to_print, BASE_10, ret_val + index_obj + strlen(flag_buff));
	return ret_val;
}

void create_str_int(print_specifiers_t *specs, va_list param, char *buff) {
	int to_print;
	size_t supposed_len;
	size_t malloc_size;
	char *ptr;

	to_print = va_arg(param, int);
	write_prefix_in_buff(to_print, NO_HASHTAG_STRING, buff, specs->flags);
	supposed_len = len_of_nb(to_print, 10) + strlen(buff);
	specs->string = build_string(specs, supposed_len, buff, to_print);
	specs->len = strlen(specs->string);
}

void create_str_unsigned(print_specifiers_t *specs, va_list param, char *buff) {
	long long to_print = va_arg(param, unsigned int);

	write_prefix_in_buff(to_print, NO_HASHTAG_STRING, buff, specs->flags);
	itoa_base(to_print, BASE_10, buff);
	specs->string = buff;
}

void create_str_bad_identifiers(print_specifiers_t *specs, va_list param, char *buff) {
	memcpy(buff, "Error format", strlen("Error format"));
	specs->string = buff;
}

void create_str_octal(print_specifiers_t *specs, va_list param, char *buff) {
	int to_print = va_arg(param, int);

	write_prefix_in_buff(to_print, "0", buff, specs->flags);
	itoa_base(to_print, BASE_8, buff);
	specs->string = buff;
}

void create_str_hexa_min(print_specifiers_t *specs, va_list param, char *buff) {
	long long to_print = va_arg(param, unsigned int);

	write_prefix_in_buff(to_print, "0x", buff, specs->flags);
	itoa_base(to_print, BASE_HEX_MIN, buff);
	specs->string = buff;
}

void create_str_pointer(print_specifiers_t *specs, va_list param, char *buff) {
	uintptr_t to_print = va_arg(param, uintptr_t);

	write_prefix_in_buff(to_print, "0x", buff, specs->flags);
	itoa_base(to_print, BASE_HEX_MIN, buff);
	specs->string = buff;
}

void create_str_percent(print_specifiers_t *specs, va_list param, char *buff) {
	*buff = '%';
	specs->string = buff;
}

void create_str_hexa_maj(print_specifiers_t *specs, va_list param, char *buff) {
	long long to_print = va_arg(param, unsigned int);

	write_prefix_in_buff(to_print, "0X", buff, specs->flags);
	itoa_base(to_print, BASE_HEX_MAJ, buff);
	specs->string = buff;
}

void create_str_character(print_specifiers_t *specs, va_list param, char *buff) {
	char to_print = va_arg(param, int);
	
	*buff = to_print;
	specs->string = buff;
}

void create_str_string(print_specifiers_t *specs, va_list param, char *buff) {
	char *to_print = va_arg(param, char *);
	specs->string = to_print;
}



size_t print_from_spec(print_specifiers_t *spec) {
	static char buffer[4096] = {};

	size_t len = spec->len;
	char *string = spec->string;

	write(1, string, len);
		
	return 0;
}

const char *ft_print_param(const char *format, va_list prms)
{
	print_specifiers_t spec;
	size_t printed_char;
	char buff[64];
	char *casted_format = (char *)format;

	bzero(buff, 64);
	
	casted_format++;
	memset(&spec, 0, sizeof(print_specifiers_t));
	casted_format = parse_flag_in_spec(&spec, casted_format);
	casted_format = parse_min_field_in_spec(&spec, casted_format);
	casted_format = parse_precision_in_spec(&spec, casted_format);
	casted_format = parse_function_in_spec(&spec, casted_format);
	((create_str_function_t*)spec.function_to_use)(&spec, prms, buff);
	spec.len = strlen(spec.string);

	print_from_spec(&spec);
	return (const char *)casted_format;
}

