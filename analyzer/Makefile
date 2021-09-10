LEXER_SRCS		:=	ms_lexer.c lx_wordutils.c lx_string_class.c lx_operator.c\
					lx_destroy.c lx_cursor.c\
					test_parser.c

PARSER_SRCS		:=	ms_parser.c pa_unit.c pa_error.c pa_factory.c\
					pa_redir.c pa_manip.c pa_unit_operator.c\
					pa_syntax_term.c pa_destroy.c

EXPANDER_SRCS	:=	ms_expander.c\
					ex_state.c ex_error.c\
					ex_destroy.c ex_cursor.c ex_print.c\
					ex_ll.c ex_lstcat.c ex_ll_trap.c ex_ll_dquote.c ex_ll_var.c\
					ex_split.c\
					ex_fx.c ex_fx_cwd.c ex_fx_match.c\
					ex_join.c\
					test_parser.c

BLT_DIR 		:=	../builtin

SRCS		:=	$(LEXER_SRCS)\
				$(PARSER_SRCS)\
				$(EXPANDER_SRCS)

CC			:= gcc
# CFLAGS		:= -g -fsanitize=address -Wall -Wextra -Werror -D PA_DEBUG=1
CFLAGS		:= -g -Wall -Wextra -Werror -D PA_DEBUG=1
OBJS		:= $(SRCS:.c=.o)
LIBFT_DIR	:= ../libft
LIBFT		:= $(LIBFT_DIR)/libft.a
NORM		:= python3 -m norminette
NAME		:= libmsa.a
BLTIN_DIR	:= ../builtin
BLTIN		:= $(BLTIN_DIR)/libmblt.a

all			: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

TEST_NAME	:= tlex
TEST_SRCS	:= test.c
TEST_OBJS	:= $(TEST_SRCS:.c=.o)

testbuild	: $(TEST_NAME)

$(TEST_NAME): $(NAME) $(TEST_OBJS) $(LIBFT) $(BLTIN)
	$(CC) $(CFLAGS) -o $(TEST_NAME) $(TEST_OBJS) $(NAME) $(LIBFT) $(BLTIN)

$(LIBFT)	:
	$(MAKE) -C $(LIBFT_DIR)

$(BLTIN)	:
	$(MAKE) -C $(BLTIN_DIR)

.PHONY		: clean
clean		:
	$(RM) $(OBJS) $(TEST_OBJS)

.PHONY		: fclean
fclean		: clean
	$(RM) $(NAME) $(TEST_NAME)

.PHONY		: re
re			: fclean all
	$(MAKE) -C $(BLTIN_DIR) fclean

.PHONY		: norm
norm		:
	$(NORM) $(LEXER_SRCS) $(PARSER_SRCS) $(EXPANDER_SRCS) *.h
	$(NORM) -v

.PHONY		: name
name		: $(NAME)
	nm -u $(NAME) | sort -u
