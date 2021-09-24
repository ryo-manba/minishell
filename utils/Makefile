NAME	:=	libmu.a

TEST_NAME	:=	tutils

SRCS		:=	exec_error.c \
				exec_expand_redir.c \
				exec_backup_fd.c \
				exec_check_path.c \
				exec_get_path.c \
				exec_main_flow.c \
				exec_pipe_cmd.c \
				exec_restore_env.c \
				exec_simple_cmd.c \
				exec_utils.c \
				exec_run.c \
				ms_pipe.c \
				ms_redir.c \
				ms_redir_error.c \
				ms_redir_heredoc.c\
				ms_signal_handler.c \


TEST_SRCS	:=	main.c
OBJS		:=	$(SRCS:.c=.o)
TEST_OBJS	:=	$(TEST_SRCS:.c=.o)
RM			:= rm -f
CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -I/usr/local/opt/readline/include/
#CFLAGS		:= -g -fsanitize=address -Wall -Wextra -Werror -I/usr/local/opt/readline/include/
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

NORM		:= python3 -m norminette
.PHONY		: norm
# norm
norm		:
	$(NORM) $(SRCS) *.h
	$(NORM) -v
