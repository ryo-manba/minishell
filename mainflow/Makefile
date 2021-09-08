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
UTILS_DIR	:= ../utils
UTILS		:= $(UTILS_DIR)/libmu.a
BLTIN_DIR	:= ../builtin
BLTIN		:= $(BLTIN_DIR)/libmblt.a
NORM		:= python3 -m norminette

all			: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(ANALYZER) $(UTILS) $(BLTIN)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(ANALYZER) $(UTILS) $(BLTIN)
$(LIBFT)	:
	$(MAKE) -C $(LIBFT_DIR)
$(ANALYZER)	:
	$(MAKE) -C $(ANALYZER_DIR)
$(UTILS)	:
	$(MAKE) -C $(UTILS_DIR)
$(BLTIN)	:
	$(MAKE) -C $(BLTIN_DIR)

.PHONY		: clean
clean		:
	$(RM) $(OBJS)

.PHONY		: fclean
fclean		: clean
	$(RM) $(NAME)
	$(MAKE) -C $(ANALYZER_DIR) fclean
	$(MAKE) -C $(UTILS_DIR) fclean
	$(MAKE) -C $(BLTIN_DIR) fclean

.PHONY		: re
re			: fclean all

.PHONY		: norm
norm		:
	$(NORM) $(SRCS) *.h
	$(NORM) -v
