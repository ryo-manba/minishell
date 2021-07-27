NAME	=	lexer
SRCS	=	lexer.c lexer_utils.c
OBJS	=	$(SRCS:.c=.o)
RM		=	rm -f
CC		= 	gcc -g #-fsanitize=address
#CFLAGS	=	-Wall -Wextra -Werror

all: $(NAME) ## Compile

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS)

clean: ## Delete *.o file
	$(RM) $(OBJS)

fclean: clean ## Delete *.o file and philo
	$(RM) $(NAME)

re: fclean all ## Recompile

test: all
	./lexer

.PHONY: all clean fclean re help sani

help:
	@grep -E '^[a-zA-Z1-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'
