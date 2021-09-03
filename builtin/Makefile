NAME	=	a.out
SRCS 	=	ms_builtin.c \
			ms_buitin_error.c \
			ms_cd.c \
			ms_echo.c \
			ms_env.c \
			ms_env_sort.c \
			ms_exit.c \
			ms_export.c \
			ms_export_check.c \
			ms_export_print.c \
			ms_export_utils.c \
			ms_pwd.c \
			ms_unset.c \
			main.c \

OBJS	=	$(SRCS:.c=.o)
RM		=	rm -f
CC		= 	gcc -g #-fsanitize=address
#CFLAGS	=	-Wall -Wextra -Werror
CFLAGS	= 	-I/usr/local/opt/readline/include/
LIBFT	=	../libft/libft.a
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -lreadline -lhistory -o $(NAME) $(LIBFT) -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: all
	./a.out

.PHONY: all clean fclean re
