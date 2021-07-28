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
	int		detail_type;
	int		token_type;
	int		i;
	int		start;
}	t_word_list;

//	word -> [echo]
//	detail -> [token]
//	token -> [word]

typedef	enum s_status
{
	TYPE_NORMAL,
	TYPE_SPACE,
	TYPE_DQUOTE,
	TYPE_SQUOTE,
	TYPE_DOLLAR,
	TYPE_PIPE,
	TYPE_AND,
	TYPE_WILD,
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	REDIRECT_HEREDOC,
	REDIRECT_APPEND,
}	t_status;

typedef	enum s_token_type
{
	RESERVED, // 予約語はいらない
	WORD,
	OPERATOR,
}	t_token_type;

typedef struct s_stack
{
	char	c;
	int		i;
	int		start;
	struct s_stack	*next;
}	t_stack;

int			check_word(char c);
char		*create_word(char *s, int start, int end);
bool		lst_push_back(t_word_list *lst, char *s, int token_type, int detail_type);
t_word_list *lst_new(char *s, int token_type, int detail_type);

