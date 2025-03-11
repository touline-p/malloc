NAME	=	libft.a

CC	=	gcc

CFLAGS	=	-Werror -Wall -Wextra -g

AR	= 	ar

ARFLAGS =	rcs

SANITY	=	-fsanitize=address

FILES	=	ft_strlcat.c \
		ft_strchr.c \
		ft_strncmp.c \
		ft_tolower.c \
		ft_strlen.c \
		ft_striteri.c \
		ft_toupper.c \
		ft_strlcpy.c \
		ft_strmapi.c \
		ft_strnstr.c \
		ft_strrchr.c \
		ft_strtrim.c \
		ft_strjoin.c \
		ft_split.c \
		ft_calloc.c \
		ft_substr.c \
		ft_strdup.c \
		ft_memmove.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memcmp.c \
		ft_memchr.c \
		ft_memset.c \
		ft_isalpha.c \
		ft_isprint.c \
		ft_isdigit.c \
		ft_isascii.c \
		ft_isalnum.c \
		ft_itoa.c \
		ft_atoi.c \
		ft_putendl_fd.c \
		ft_putstr_fd.c \
		ft_putchar_fd.c \
		ft_putnbr_fd.c \
		ft_swap.c \
		ft_memdup.c \
	
BFILES	=	ft_lstiter.c \
		ft_lstnew.c \
		ft_lstdelone.c \
		ft_lstadd_front.c \
		ft_lstmap.c \
		ft_lstclear.c \
		ft_lstlast.c \
		ft_lstsize.c \
		ft_lstadd_back.c \

HFILES	=	libft.h\

ODIR	=	./

HDIR	=	./

SRCS	=	$(FILES)

OBJS	=	$(addprefix $(ODIR), $(FILES:.c=.o))

BOBJS	=	$(addprefix $(ODIR), $(BFILES:.c=.o))

all	:	 $(ODIR) $(NAME)

$(NAME)	:	$(OBJS) 
	$(AR) $(ARFLAGS) $(NAME) $(OBJS) 
	ranlib $(NAME)

$(ODIR)	:
	mkdir $(ODIR)

$(ODIR)%.o	:	%.c $(HFILES)
	$(CC) $(CFLAGS) -I $(HDIR) -o $@ -c $<

clean	:
	-rm -rf $(OBJS)
	-rm -rf $(BOBJS)

fclean	:	clean
	-rm $(NAME)

re	:	fclean all

bonus	:	$(BOBJS) $(OBJS) $(HEADER)
	$(AR) $(ARFLAGS) $(NAME) $(OBJS) $(BOBJS)
	ranlib $(NAME)

.PHONY	:	all clean fclean re ncpy bonus
