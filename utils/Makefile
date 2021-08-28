NAME	=	a.out
SRCS	=	main.c \
			ms_execute_command.c \
			ms_get_execution_path.c \
			ms_pipe.c \
			ms_redirect.c \
			ms_redirect_heredoc.c \

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
