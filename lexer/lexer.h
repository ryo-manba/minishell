#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct s_word_desc
{
	char	*word;
	int		flags;
}	t_word_desc;

typedef struct s_word_list
{
	struct s_word_list	*next;
	char	*word;
	int		flags;
}	t_word_list;

typedef	enum s_status
{
	TYPE_DQUOTE,
	TYPE_SQUOTE,
	TYPE_NORMAL,
	TYPE_DOLLAR,
	TYPE_SPACE,
	TYPE_PIPE,
	TYPE_AND,
	TYPE_WILD,
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	REDIRECT_HEREDOC,
	REDIRECT_APPEND,
}	t_status;

typedef	enum s_flags
{
	RESERVED,
	WORD,
	OPERATOR,
}	t_flags;

typedef struct s_stack
{
	char	c;
	int		i;
	int		start;
	struct s_stack	*next;
}	t_stack;

int			sta_get_size(t_stack *sta);
int			check_word(char c);
char		sta_pop_front(t_stack **sta);
char		*create_word(t_stack *sta, int start, int end);
bool		lst_push_back(t_word_list *lst, char *s, int flag);
bool		sta_push_back(t_stack *sta, char c);
t_stack		*stack_new(char c);
t_word_list *wordlst_new(char *s, int flag);
