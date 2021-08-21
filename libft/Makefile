# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmatsuka < rmatsuka@student.42tokyo.jp>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/07 12:59:59 by rmatsuka          #+#    #+#              #
#    Updated: 2021/07/23 22:06:50 by rmatsuka         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libft.a
SRCS	=	ft_strlen.c \
			ft_strcmp.c \
			ft_strncmp.c \
			ft_atoi.c \
			ft_isalpha.c \
			ft_isdigit.c \
			ft_isalnum.c \
			ft_isascii.c \
			ft_isprint.c \
			ft_toupper.c \
			ft_tolower.c \
			ft_memcmp.c \
			ft_strdup.c \
			ft_substr.c \
			ft_strchr.c \
			ft_strrchr.c \
			ft_strnstr.c \
			ft_strjoin.c \
			ft_memset.c \
			ft_memmove.c \
			ft_memcpy.c \
			ft_memccpy.c \
			ft_memchr.c \
			ft_calloc.c \
			ft_bzero.c \
			ft_strlcpy.c \
			ft_strlcat.c \
			ft_strtrim.c \
			ft_split.c \
			ft_itoa.c \
			ft_strmapi.c \
			ft_putchar_fd.c \
			ft_putstr_fd.c \
			ft_putendl_fd.c \
			ft_putnbr_fd.c \
			ft_starts_with.c \
			free_strjoin.c \
			safe_free.c \
			get_next_line.c

SRCS_B	=	ft_lstnew.c \
			ft_lstadd_front.c \
			ft_lstsize.c \
			ft_lstlast.c \
			ft_lstadd_back.c \
			ft_lstdelone.c \
			ft_lstclear.c \
			ft_lstiter.c \
			ft_lstmap.c \
			ft_lstpush_back.c \

OBJS	=	$(SRCS:.c=.o)
OBJS_B	=	$(SRCS_B:.c=.o)
RM		=	rm -f
LIBC	=	ar rcs
CC		= 	gcc
CFLAGS	=	-Wall -Wextra -Werror


all:		$(NAME)

bonus:		$(NAME) $(OBJS_B)
			$(LIBC) $(NAME) $(OBJS_B)

$(NAME):	$(OBJS)
			$(LIBC) $(NAME) $(OBJS)

clean:
		$(RM) $(OBJS) $(OBJS_B)

fclean:		clean
			$(RM) $(NAME) $(bonus)

re:			fclean all

.PHONY:		all clean fclean re bonus
