#ifndef LEXER_H
# define LEXER_H
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>

typedef struct s_word_list
{
	struct s_word_list	*next;
	char	*word;
	int		detail_type;
	int		token_type;
	int		i;
	int		start;
}	t_word_list;

typedef	enum s_status
{
	TYPE_NORMAL,
	TYPE_SPACE,
	TYPE_SINGLE_QUOTE,
	TYPE_DOUBLE_QUOTE,
	TYPE_AND,
	TYPE_PIPE,
	TYPE_WILD,
	TYPE_DOLLAR,
	TYPE_DOUBLE_AND,
	TYPE_DOUBLE_PIPE,
	TYPE_HEREDOC,
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	REDIRECT_APPEND,
}	t_status;

typedef	enum s_token_type
{
	RESERVED, // 予約語いらない
	WORD,
	OPERATOR,
}	t_token_type;

/* lexer */
t_word_list *lexer(char *s);
bool 		update_lst_accordingly_type(t_word_list *lst, char *s, int type);
void		check_operator_and_create_lst(t_word_list *lst, char *s, int type);
void 		free_head_lst(t_word_list **lst);
bool		check_delimiter(char *s, int i);

/* lexer_utils */
int			check_word(char c);
int			check_type(int type);
char		*create_word(char *s, int start, int end);
bool		lst_push_back(t_word_list *lst, char *s, int token_type, int detail_type);
t_word_list *lst_new(char *s, int token_type, int detail_type);

#endif
