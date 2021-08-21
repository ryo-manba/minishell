SRCS		:= ms_lexer.c ms_wordutils.c ms_string_class.c
CC			:= gcc
CFLAGS		:= -fsanitize=address -Wall -Wextra -Werror
OBJS		:= $(SRCS:.c=.o)
LIBFT_DIR	:= ../libft
LIBFT		:= $(LIBFT_DIR)/libft.a

TEST_NAME	:= tlex
TEST_SRCS	:= test.c
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
