LEXER_SRCS	:=	ms_lexer.c ms_wordutils.c ms_string_class.c ms_operator.c

PARSER_SRCS	:=	ms_parser.c ms_parse_unit.c ms_parse_error.c ms_parse_factory.c\
				ms_parse_redir.c ms_parse_manip.c ms_parse_unit_operator.c\
				ms_parse_syntax_term.c ms_parse_cleaner.c

EXPANDER_SRCS	:=	ms_expander.c ex_error.c ex_shell_param.c ex_lstcat.c\
					ex_vars.c ex_sp_opener.c ex_sp_closer.c

SRCS		:=	$(LEXER_SRCS)\
				$(PARSER_SRCS)\
				$(EXPANDER_SRCS)

CC			:= gcc
CFLAGS		:= -fsanitize=address -Wall -Wextra -Werror
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
	$(RM) $(OBJS)

.PHONY		: fclean
fclean		: clean
	$(RM) $(NAME)

.PHONY		: re
re			: fclean test_build
