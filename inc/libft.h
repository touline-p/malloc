/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoumeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 13:23:19 by bpoumeau          #+#    #+#             */
/*   Updated: 2022/11/11 13:41:27 by bpoumeau         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>

int		isalnum_ft(int c);
int		isalpha_ft(int c);
int		isascii_ft(int c);
int		isdigit_ft(int c);
int		isprint_ft(int c);
void	bzero_ft(void *dst, size_t n);
void	*memchr_ft(void *buf, int c, size_t length);
int		memcmp_ft(const void *s1, const void *s2, size_t lenght);
void	*memcpy_ft(void *dst, const void *src, size_t lenght);
void	*memmove_ft(void *dst, const void *src, size_t length);
void	*memset_ft(void *dst, int c, size_t length);
void	*memset_schema_ft(void *dst, void *schema, size_t size_dst, size_t size_src);
void	putchar_fd_ft(char c, int fd);
void	putstr_fd_ft(char *c, int fd);
void	putendl_fd_ft(char *c, int fd);
void	putnbr_fd_ft(int c, int fd);
char	*strchr_ft(const char *s, int c);
void	striteri_ft(char *s, void (*f)(unsigned int, char*));
size_t	strlcat_ft(char *dst, const char *src, size_t size);
size_t	strlcpy_ft(char *dst, const char *src, size_t size);
size_t	strlen_ft(const char *s);
char	*strmapi_ft(char const *s, char (*f)(unsigned int, char));
int		strncmp_ft(char *s1, char *s2, size_t n);
char	*strnstr_ft(char *big, const char *little, size_t len);
char	*strrchr_ft(const char *s, int c);
int		tolower_ft(int c);
int		toupper_ft(int c);
void	*calloc_ft(size_t n, size_t size);
char	**split_ft(char const *s, char c);
char	*strdup_ft(const char *src);
char	*strjoin_ft(char const *s1, char const *s2);
char	*strtrim_ft(char const *s1, char const *set);
char	*substr_ft(char const *s, unsigned int start, size_t len);
int		atoi_ft(const char *nptr);
int		atoi_has_overflown_ft(char *str);
char	*itoa_ft(int n);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
void		lstadd_front_ft(t_list **lst, t_list *new);
void		lstadd_back_ft(t_list **lst, t_list *new);
void		lstclear_ft(t_list **lst, void (*del)(void*));
void		lstdelone_ft(t_list *lst, void (*del)(void*));
void		lstiter_ft(t_list *lst, void (*f)(void*));
t_list		*lstlast_ft(t_list *lst);
t_list		*lstmap_ft(t_list *lst, void *(*f)(void*), void(*del)(void*));
t_list		*lstnew_ft(void *content);
int			lstsize_ft(t_list *lst);

void		*memdup_ft(void *src, size_t ln);
void		swap_ft(int *a, int *b);
#endif
