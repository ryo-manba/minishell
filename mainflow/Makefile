NAME		:=	minishell
SRCS		:=	ms_main.c\
				mf_repl.c\
				mf_lpx.c\
				mf_opt.c\
				mf_error.c\

CC			:= gcc
CFLAGS		:= -g -fsanitize=address -Wall -Wextra -Werror -I/usr/local/opt/readline/include/
# CFLAGS		:= -g -Wall -Wextra -Werror -D PA_DEBUG=1
ifdef MS_DEBUG
	CFLAGS	+= -D MS_DEBUG=$(MS_DEBUG)
endif
LFLAGS		:= -lreadline -lhistory
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

INC			:= -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include

all			: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(ANALYZER) $(UTILS) $(BLTIN)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(NAME) $(INC) $(OBJS) $(LIBFT) $(ANALYZER) $(UTILS) $(BLTIN)
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
# fclean all submodules without libft and rebuild.
re			: fclean all

.PHONY		: reft
# fclean all submodules including libft and rebuild.
reft		:
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) re

.PHONY		: runtest
# test
runtest		: $(NAME)
	cd test; bash runner.sh

.PHONY		: norm
# norm
norm		:
	$(NORM) $(SRCS) *.h
	$(NORM) -v

.PHONY		: normall
# norm all submodules
normall		:
	$(NORM) $(SRCS) *.h
	$(MAKE) -C $(LIBFT_DIR) norm
	$(MAKE) -C $(ANALYZER_DIR) PA_DEBUG=1 norm
	$(MAKE) -C $(UTILS_DIR) norm
	$(MAKE) -C $(BLTIN_DIR) norm
	$(NORM) -v

.PHONY		: name
name		: $(NAME)
	nm -u $(NAME) | sort -u
