LEXER_SRCS		:=	ms_lexer.c ms_wordutils.c ms_string_class.c ms_operator.c\
					lx_destroy.c

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

EXT_SRCS		:= ../builtin/ms_env.c ../builtin/ms_env_utils.c

SRCS		:=	$(LEXER_SRCS)\
				$(PARSER_SRCS)\
				$(EXPANDER_SRCS)\
				$(EXT_SRCS)

CC			:= gcc
# CFLAGS		:= -g -fsanitize=address -Wall -Wextra -Werror
CFLAGS		:= -g -Wall -Wextra -Werror
OBJS		:= $(SRCS:.c=.o)
LIBFT_DIR	:= ../libft
LIBFT		:= $(LIBFT_DIR)/libft.a

TEST_NAME	:= tlex
TEST_SRCS	:= test.c test_parser.c
TEST_OBJS	:= $(TEST_SRCS:.c=.o)

test_build	: $(TEST_NAME)

$(TEST_NAME): $(TEST_OBJS) $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(TEST_NAME) $(TEST_OBJS) $(OBJS) $(LIBFT)
$(LIBFT)	:
	$(MAKE) -C $(LIBFT_DIR)

.PHONY		: clean
clean		:
	$(RM) $(OBJS) $(TEST_OBJS)

.PHONY		: fclean
fclean		: clean
	$(RM) $(NAME)

.PHONY		: re
re			: fclean test_build
