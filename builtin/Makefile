NAME		:=	libmblt.a
TEST_NAME	:=	a.out
SRCS 		:=	blt_cd_lst.c \
				blt_cd_no_current.c \
				blt_cd_normalize_path.c \
				blt_cd_normalize_utils.c \
				blt_cd_update_pwd.c \
				blt_cd.c \
				blt_echo.c \
				blt_env.c \
				blt_env_sort.c \
				blt_exit.c \
				blt_export.c \
				blt_export_check.c \
				blt_export_print.c \
				blt_export_utils.c \
				blt_pwd.c \
				blt_unset.c \
				ms_builtin.c \
				ms_create_env.c \
				ms_env_utils.c\
				ms_env_preset.c\
				../analyzer/lx_string_class.c\

TEST_SRCS	:=	main.c

OBJS		:=	$(SRCS:.c=.o)
TEST_OBJS	:=	$(TEST_SRCS:.c=.o)
RM			:=	rm -f
CC			:= 	gcc
CFLAGS		:=	-g -Wall -Wextra -Werror
#CFLAGS		:=	-g -fsanitize=address -Wall -Wextra -Werror
LIBFT_DIR	:=	../libft
LIBFT		:=	$(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

$(TEST_NAME): $(OBJS) $(TEST_OBJS) $(LIBFT)
	$(CC) -lreadline -lhistory -o $(TEST_NAME) $(LIBFT) -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include $(OBJS)

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
