/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 22:47:43 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/22 23:31:47 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <dlfcn.h>

# include <stdbool.h>
# define OUTPUT_STREAM 1
# include <stdarg.h>
# include <unistd.h>
# define RETURN_PRINT_LEN 42

# if defined(_WIN32) || defined(_WIN64)
#  define LIB_B_API __declspec(dllexport)
# else
#  define LIB_B_API __attribute__((visibility("default")))
# endif

# ifndef OUTPUT_ERR_FD
#  define OUTPUT_ERR_FD 2
# endif

# define PRINTF_FT_LIB "libprintf_ft.so"
# define PRINTF_FT_NAME "printf_ft"


extern int printf_ft(const char *format, ...);

const char *ft_print_param(const char *format, va_list prms);

void	ft_c(va_list prms, unsigned long *ptr_cnt);
void	ft_p(va_list prms, unsigned long *ptr_cnt);
void	ft_s(va_list prms, unsigned long *ptr_cnt);
void	ft_u(va_list prms, unsigned long *ptr_cnt);
void	ft_x(va_list prms, unsigned long *ptr_cnt);
void	ft_d(va_list prms, unsigned long *ptr_cnt);
void	ft_x_up(va_list prms, unsigned long *ptr_cnt);
void	ft_percent(unsigned long *ptr_cnt);

# define PRINTF_FLAGS "-0+ #"
enum print_flags_e {
	LEFT_JUSTIFY,
	ZERO_PADDING,
	PLUS_DEFAULT,
	BLANK,
	HASHTAG,
	PRINT_FLAG_NB,
};

# define FORMAT_IDENTIFIERS "dioxXucspn%"
typedef enum format_identifiers_e {
	d_FRMT,
	i_FRMT,
	o_FRMT,
	x_FRMT,
	X_FRMT,
	u_FRMT,
	c_FRMT,
	s_FRMT,
	p_FRMT,
	PERCENT_FRMT,
	BAD_FORMAT,
	FRMT_NB
} format_identifiers_t;

typedef struct print_specifiers_s {
	bool flags[PRINT_FLAG_NB];
	va_list arg;
	void *print_function;
	size_t min_field_width;
	size_t precision;
	void *function_to_use;
	enum format_identifiers_e identifiers;
	char *string;
	size_t len;
} print_specifiers_t;

typedef void create_str_function_t(print_specifiers_t *, va_list param, char *buff);
size_t print_from_spec(print_specifiers_t *spec);

#endif
