NAME	=	pipe
SRCS	=	ms_pipe.c ms_redirect.c main.c
OBJS	=	$(SRCS:.c=.o)
RM		=	rm -f
CC		= 	gcc -g #-fsanitize=address
#CFLAGS	=	-Wall -Wextra -Werror
CFLAGS	= 	-I/usr/local/opt/readline/include/

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -lreadline -lhistory -o $(NAME) libft.a -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: all
	./pipe

.PHONY: all clean fclean re
