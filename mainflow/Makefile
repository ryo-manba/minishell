NAME		:=	minishell
SRCS		:=	ms_main.c

CC			:= gcc
# CFLAGS		:= -g -fsanitize=address -Wall -Wextra -Werror -D PA_DEBUG=1
CFLAGS		:= -g -Wall -Wextra -Werror -D PA_DEBUG=1
LFLAGS		:= -lreadline
OBJS		:= $(SRCS:.c=.o)
LIBFT_DIR	:= ../libft
LIBFT		:= $(LIBFT_DIR)/libft.a
ANALYZER_DIR	:= ../analyzer
ANALYZER	:= $(ANALYZER_DIR)/libmsa.a
NORM		:= python3 -m norminette

all			: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(ANALYZER)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(ANALYZER)
$(LIBFT)	:
	$(MAKE) -C $(LIBFT_DIR)
$(ANALYZER)	:
	$(MAKE) -C $(ANALYZER_DIR)

.PHONY		: clean
clean		:
	$(RM) $(OBJS)

.PHONY		: fclean
fclean		: clean
	$(RM) $(NAME)
	$(MAKE) -C $(ANALYZER_DIR) fclean

.PHONY		: re
re			: fclean all

.PHONY		: norm
norm		:
	$(NORM) $(SRCS) *.h
	$(NORM) -v
