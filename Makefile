NAME		:=	minishell
SRCS		:=	ms_main.c\
				mf_repl.c\
				mf_lpx.c\
				mf_opt.c\
				mf_error.c\

ifndef RL_PATH
	RL_PATH	:=/usr/local/opt/readline
endif

CC			:= gcc
ifdef SANITIZE
	CFLAGS		:= -g -fsanitize=address -Wall -Wextra -Werror
else
	CFLAGS		:= -Wall -Wextra -Werror
endif
CFLAGS			+= -I$(RL_PATH)/include/
ifdef MS_DEBUG
	CFLAGS	+= -D MS_DEBUG=$(MS_DEBUG)
	ANALYZER_DEFS	:= "PA_DEBUG=1"
endif
LFLAGS			:= -lreadline -lhistory
OBJS			:= $(SRCS:.c=.o)
LIBFT_DIR		:= ./libft
LIBFT			:= $(LIBFT_DIR)/libft.a
ANALYZER_DIR	:= ./analyzer
ANALYZER		:= $(ANALYZER_DIR)/libmsa.a
UTILS_DIR		:= ./utils
UTILS			:= $(UTILS_DIR)/libmu.a
BLTIN_DIR		:= ./builtin
BLTIN			:= $(BLTIN_DIR)/libmblt.a
NORM			:= python3 -m norminette

INC				:= -L$(RL_PATH)/lib -I$(RL_PATH)/include

all				: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(ANALYZER) $(UTILS) $(BLTIN)
	$(CC) $(CFLAGS) $(INC) $(LFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(ANALYZER) $(UTILS) $(BLTIN)
$(LIBFT)	:
	$(MAKE) -C $(LIBFT_DIR) CFLAGS="$(CFLAGS)"
$(ANALYZER)	:
	$(MAKE) -C $(ANALYZER_DIR) $(ANALYZER_DEFS) CFLAGS="$(CFLAGS)"
$(UTILS)	:
	$(MAKE) -C $(UTILS_DIR) RL_PATH=$(RL_PATH) CFLAGS="$(CFLAGS)"
$(BLTIN)	:
	$(MAKE) -C $(BLTIN_DIR) CFLAGS="$(CFLAGS)"

.PHONY		: clean
clean		:
	$(RM) $(OBJS)

.PHONY		: allclean
allclean	:
	$(RM) $(OBJS)
	$(MAKE) -C $(ANALYZER_DIR) clean
	$(MAKE) -C $(UTILS_DIR) clean
	$(MAKE) -C $(BLTIN_DIR) clean
	$(MAKE) -C $(LIBFT_DIR) clean

.PHONY		: fclean
fclean		: clean
	$(RM) $(NAME)
	$(MAKE) -C $(ANALYZER_DIR) fclean
	$(MAKE) -C $(UTILS_DIR) fclean
	$(MAKE) -C $(BLTIN_DIR) fclean
	$(MAKE) -C $(LIBFT_DIR) fclean

.PHONY		: re
# fclean all submodules without libft and rebuild.
re			: fclean all

.PHONY		: dere
dere		:
	$(MAKE) MS_DEBUG=1 re

.PHONY		: reft
# fclean all submodules including libft and rebuild.
reft		:
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) re

.PHONY		: runtest
# test
runtest		: $(NAME)
	bash test/runner.sh

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
	$(MAKE) -C $(ANALYZER_DIR) norm
	$(MAKE) -C $(UTILS_DIR) norm
	$(MAKE) -C $(BLTIN_DIR) norm
	$(NORM) -v

.PHONY		: name
name		: $(NAME)
	nm -u $(NAME) | sort -u
