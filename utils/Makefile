NAME	:=	libmu.a

TEST_NAME	:=	tutils

SRCS		:=	ms_execute_pipe_command.c \
				ms_execute_simple_command.c \
				ms_execute_utils.c \
				ms_executer.c \
				ms_get_execution_path.c \
				ms_pipe.c \
				ms_redirect.c \
				ms_redirect_heredoc.c

TEST_SRCS	:=	main.c
OBJS		:=	$(SRCS:.c=.o)
TEST_OBJS	:=	$(TEST_SRCS:.c=.o)
RM			:= rm -f
CC			:= gcc
#CFLAGS	=	-Wall -Wextra -Werror
CFLAGS		:= -g -I/usr/local/opt/readline/include/
LIBFT_DIR	:=	../libft
LIBFT		:=	$(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

$(TEST_NAME): $(OBJS) $(TEST_OBJS) $(LIBFT)
	$(CC) -lreadline -lhistory -o $(NAME) $(LIBFT) -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include $(OBJS)

$(LIBFT)	:
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: all
	./$(TEST_NAME)

.PHONY: all clean fclean re
